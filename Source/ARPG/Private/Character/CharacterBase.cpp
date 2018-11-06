// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include <Engine/World.h>
#include <Animation/AnimInstance.h>
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>
#include "XD_CharacterMovementComponent.h"
#include "ARPG_MovementComponent.h"
#include "ARPG_InventoryComponent.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_Battle_Log.h"
#include "ReceiveDamageActionBase.h"
#include <Kismet/KismetMathLibrary.h>
#include "ARPG_DebugFunctionLibrary.h"
#include "ARPG_LevelFunctionLibrary.h"
#include "XD_TemplateLibrary.h"
#include "UnrealNetwork.h"
#include "ARPG_ActorFunctionLibrary.h"


// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ARPG_MovementComponent = CastChecked<UARPG_MovementComponent>(GetCharacterMovement());
	{
		bUseControllerRotationYaw = false;
	}

	Inventory = CreateDefaultSubobject<UARPG_InventoryComponent>(GET_MEMBER_NAME_CHECKED(ACharacterBase, Inventory));
	{

	}

	DodgeAnimSet = CreateDefaultSubobject<UARPG_DodgeAnimSetNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, DodgeAnimSet));

	GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//删除角色身上的东西
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* Actor : AttachedActors)
	{
		Actor->Destroy();
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO 挪入bIsLockingOther与bInvokeSprint的Setter驱动而不是轮询
	ARPG_MovementComponent->SetRotationMode(bIsLockingOther && ARPG_MovementComponent->bInvokeSprint == false ? ECharacterRotationMode::LookingDirection : ECharacterRotationMode::VelocityDirection);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, bIsLockingOther);
}

void ACharacterBase::WhenGameInit_Implementation()
{
	//初始化道具和使用中的道具
	{
		TArray<UARPG_ItemCoreBase*> RemoveItems;
		TArray<FARPG_Item> FinalInitItemList = GetInitItemList();
		//对已存在的进行数量修改
		for (UARPG_ItemCoreBase* ItemCore : Inventory->GetItemCoreList())
		{
			if (const FARPG_Item* Item = FinalInitItemList.FindByPredicate([ItemCore](const FARPG_Item& E_Item) {return ItemCore->EqualForItemCore(E_Item.ItemCore); }))
			{
				ItemCore->Number = Item->ItemCore->Number;
				FinalInitItemList.RemoveAll([&](const FARPG_Item& E) {return E->EqualForItemCore(ItemCore); });
			}
			else
			{
				RemoveItems.Add(ItemCore);
			}
		}
		//移除初始化列表中不存在的
		for (UARPG_ItemCoreBase* ItemCore : RemoveItems)
		{
			Inventory->ItemCoreList.Remove(ItemCore);
		}
		//添加需要添加的
		Inventory->AddItemArray(FinalInitItemList);
		Inventory->OnRep_ItemList();
	}

	//初始化角色坐标
	{
		BornLocation = GetActorLocation();
		BornWorldOrigin = UGameplayStatics::GetWorldOriginLocation(this);
	}
}

TArray<struct FARPG_Item> ACharacterBase::GetInitItemList() const
{
	TArray<FARPG_Item> Res = ReceivedGetInitItemList();
	Res.Append(ArrayCast<FARPG_Item>(Inventory->InitItems));
	return Res;
}

void ACharacterBase::SetRebornLocation(const FVector& RebornLocation)
{
	BornLocation = RebornLocation;
	BornWorldOrigin = GetWorld()->OriginLocation;
}

FVector ACharacterBase::GetRebornLocation()
{
	return UARPG_LevelFunctionLibrary::GetFixedWorldLocation(this, BornWorldOrigin, BornLocation);
}

void ACharacterBase::ARPG_InputPressed(EARPG_InputType InputType)
{
	InputBuffer.InputPressed(InputType);
}

void ACharacterBase::ARPG_InputReleased(EARPG_InputType InputType)
{
	InputBuffer.InputReleased(InputType);
}

bool ACharacterBase::ARPG_InputIsPressed(int32 InputType) const
{
	return InputBuffer.IsPressed(InputType);
}

bool ACharacterBase::ARPG_InputIsReleased(int32 InputType) const
{
	return InputBuffer.IsReleased(InputType);
}

bool ACharacterBase::ARPG_AnyInputIsPressed(int32 InputType) const
{
	return InputBuffer.IsAnyPressed(InputType);
}

bool ACharacterBase::ARPG_AllInputIsReleased(int32 InputType) const
{
	return InputBuffer.IsAllReleased(InputType);
}

void ACharacterBase::InvokeChangeMoveGait(ECharacterGait Gait)
{
	if (ARPG_MovementComponent->CurrentGait != Gait)
	{
		InvokeChangeMoveGaitToServer_Implementation(Gait);
		InvokeChangeMoveGaitToServer(Gait);
	}
}

void ACharacterBase::InvokeChangeMoveGaitToServer_Implementation(const ECharacterGait& Gait)
{
	ARPG_MovementComponent->bInvokeSprint = (Gait == ECharacterGait::Sprinting);
	ARPG_MovementComponent->SetGait(Gait);
}

FVector ACharacterBase::GetTargetLocation_Implementation(const FName& CurLockSocketName) const
{
	return GetMesh()->GetSocketLocation(CurLockSocketName);
}

bool ACharacterBase::CanLockedOnTarget_Implementation(AController* Invoker, const FName& InvokeLockedSocketName) const
{
	bool TryLockAgain;
	if (CanLockingOnTarget_Implementation(Invoker, InvokeLockedSocketName, TryLockAgain))
	{
		if (CanLockedSocketNames.Contains(InvokeLockedSocketName))
		{
			return true;
		}
	}
	return false;
}

bool ACharacterBase::CanLockingOnTarget_Implementation(AController* Invoker, const FName& CurLockSocketName, bool& TryLockAgain) const
{
	if (APawn* Pawn = Invoker->GetPawn())
	{
		if ((Pawn->GetActorLocation() - GetActorLocation()).Size() > MaxLockingDistance)
		{
			TryLockAgain = false;
			return false;
		}

		return true;
	}
	return false;
}

float ACharacterBase::PlayMontage(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/, bool ClientMaster /*= true*/)
{
	if (MontageToPlay)
	{
		if (HasAuthority())
		{
			MulticastPlayMontage(MontageToPlay, InPlayRate, StartSectionName);
		}
		else if (ClientMaster)
		{
			if (HasAuthority())
			{
				if (GetController() && GetController()->IsLocalController())
				{
					MulticastPlayMontage(MontageToPlay, InPlayRate, StartSectionName);
				}
			}
			else
			{
				PlayMontageImpl(MontageToPlay, InPlayRate, StartSectionName);
				PlayMontageToServer(MontageToPlay, InPlayRate, StartSectionName);
			}
		}
		return MontageToPlay->GetPlayLength();
	}
	return 0.f;
}

float ACharacterBase::PlayMontageImpl(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	UAnimInstance * AnimInstance = GetMesh()->GetAnimInstance();
	if (MontageToPlay && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(MontageToPlay, InPlayRate, EMontagePlayReturnType::MontageLength, 0.f, true);

		if (Duration > 0.f)
		{
			// Start at a given Section.
			if (StartSectionName != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, MontageToPlay);
			}

			return Duration;
		}
	}

	return 0.f;
}

void ACharacterBase::MulticastPlayMontage_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	PlayMontageImpl(MontageToPlay, InPlayRate, StartSectionName);
}

void ACharacterBase::MulticastPlayMontageSkipOwner_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	if (IsLocallyControlled() == false || GetMesh()->GetAnimInstance()->Montage_IsPlaying(MontageToPlay) == false)
	{
		PlayMontageImpl(MontageToPlay, InPlayRate, StartSectionName);
	}
}

void ACharacterBase::StopMontage_Implementation()
{
	StopAnimMontage();
}

void ACharacterBase::PlayMontageToServer_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	MulticastPlayMontageSkipOwner(MontageToPlay, InPlayRate, StartSectionName);
}

void ACharacterBase::TryPlayMontage(const FARPG_MontageParameter& Montage)
{
	if (Montage.Condition == nullptr || Montage.Condition.GetDefaultObject()->CanPlayMontage(this))
	{
		PlayMontage(Montage.Montage, 1.f, NAME_None, true);
	}
}

bool ACharacterBase::CanPlayFullBodyMontage() const
{
	return GetMesh()->GetAnimInstance()->GetSlotMontageGlobalWeight(FullBodySlotName) == 0.f;
}

void ACharacterBase::InvokeDodge()
{
	if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Dodge)))
	{
		if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Forward)))
		{
			DodgeByControlRotation(0.f);
		}
		else if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Backward)))
		{
			DodgeByControlRotation(-180.f);
		}
		else if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Left)))
		{
			DodgeByControlRotation(-90.f);
		}
		else if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Right)))
		{
			DodgeByControlRotation(90.f);
		}
	}
}

void ACharacterBase::DodgeByControlRotation(float Direction)
{
	FRotator ControllerRotation = GetControlRotation();
	ControllerRotation.Yaw += Direction;
	float DodgeDirection = (GetActorRotation() - ControllerRotation).GetNormalized().Yaw;
	if (DodgeDirection >= -45.f && DodgeDirection <= 45.f)
	{
		InvokeDodgeByDirection(EDodgeDirection::Forward);
	}
	else if (DodgeDirection >= -135.f && DodgeDirection < -45.f)
	{
		InvokeDodgeByDirection(EDodgeDirection::Right);
	}
	else if (DodgeDirection > 45.f && DodgeDirection <= 135.f)
	{
		InvokeDodgeByDirection(EDodgeDirection::Left);
	}
	else
	{
		InvokeDodgeByDirection(EDodgeDirection::Backword);
	}
}

void ACharacterBase::InvokeDodgeByDirection(EDodgeDirection Direction)
{
	DodgeAnimSet->InvokeDodge(this, Direction);
}

bool ACharacterBase::CanDodge() const
{
	return DodgeAnimSet->CanDodge(this);
}

void ACharacterBase::MoveItem_Implementation(class UARPG_InventoryComponent* SourceInventory, class UARPG_InventoryComponent* TargetInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	TargetInventory->GetItemFromOther(SourceInventory, ItemCore, Number);
}

void ACharacterBase::TradeItem_Implementation(class UARPG_InventoryComponent* TraderInventory, class UARPG_InventoryComponent* BuyerInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	if (BuyerInventory == Inventory)
	{
		Inventory->BuyItemFromOther(TraderInventory, ItemCore, Number);
	}
	else
	{
		Inventory->SellItemToOther(TraderInventory, ItemCore, Number);
	}
}

void ACharacterBase::InvokeUseItem_Implementation(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput)
{
	if (ARPG_MovementComponent->IsMovingOnGround() && CanPlayFullBodyMontage())
	{
		UseItemImmediately(ItemCore, UseItemInput);
	}
}

void ACharacterBase::UseItemImmediately(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput)
{
	if (!ItemCore)
		return;
	for (UARPG_ItemCoreBase* E_ItemCore : Inventory->GetItemCoreList())
	{
		if (E_ItemCore->EqualForItemCore(ItemCore))
		{
			OnUseItem.Broadcast(this, ItemCore, UseItemInput);
			E_ItemCore->UseItem(this, UseItemInput);
			break;
		}
	}
}

void ACharacterBase::NearAttackSuccessTimeDilation(float Duration)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(AttackSuccessTimeDilation_TimeHandle))
	{
		CustomTimeDilation *= AttackSuccessTimeDilationRate;
	}
	GetWorld()->GetTimerManager().SetTimer(AttackSuccessTimeDilation_TimeHandle, this, &ACharacterBase::RecoverAttackSuccessTimeDilation, Duration, false);
}

void ACharacterBase::RecoverAttackSuccessTimeDilation()
{
	CustomTimeDilation /= AttackSuccessTimeDilationRate;
}

void ACharacterBase::WhenDodgeSucceed(float BaseDamage, class ACharacterBase* InstigatedBy, const FHitResult& HitResult)
{
	Battle_Display_LOG("%s成功闪避%s的攻击", *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(InstigatedBy));
	ReceiveWhenDodgeSucceed(BaseDamage, InstigatedBy, HitResult);
	OnDodgeSucceed.Broadcast(this, BaseDamage, InstigatedBy, HitResult);
}

bool ACharacterBase::IsDefenseSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const
{
	return bIsDefense;
}

void ACharacterBase::WhenDefenseSucceed(float BaseDamage, class ACharacterBase* InstigatedBy, const FVector& HitFromDirection, float DefenseBeakBackDistance, const FHitResult& HitResult)
{
	Battle_Display_LOG("%s成功防御%s的攻击", *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(InstigatedBy));
	ReceiveWhenDefenseSucceed(BaseDamage, InstigatedBy, HitFromDirection, DefenseBeakBackDistance, HitResult);
	OnDefenseSucceed.Broadcast(this, BaseDamage, InstigatedBy, HitResult);

	//击退效果
	FVector BeakBackOffset = HitFromDirection.GetSafeNormal2D() * DefenseBeakBackDistance;
	UARPG_ActorFunctionLibrary::PushActorTo(this, BeakBackOffset);
}

void ACharacterBase::WhenAttackedDefenseCharacter(float BaseDamage, ACharacterBase* DefenseSucceedCharacter, const FHitResult& HitResult)
{
	ReceiveWhenAttackedDefenseCharacter(BaseDamage, DefenseSucceedCharacter, HitResult);
}

bool ACharacterBase::IsDefenseSwipeSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const
{
	return bIsDefenseSwipe;
}

float ACharacterBase::AddHitStun(float Value)
{
	HitStunValue += Value;
	if (HitStunValue >= ToughnessValue)
	{
		float OverflowValue = HitStunValue - ToughnessValue;
		HitStunValue = 0;
		GetWorld()->GetTimerManager().ClearTimer(ClearHitStun_TimeHandle);
		return OverflowValue;
	}
	GetWorld()->GetTimerManager().SetTimer(ClearHitStun_TimeHandle, this, &ACharacterBase::ClearHitStun, HitStunClearTime);
	return -1;
}

void ACharacterBase::ClearHitStun()
{
	HitStunValue = 0;
}

bool ACharacterBase::CanBeBackstab(ACharacterBase* BackstabInvoker) const
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(BackstabInvoker->GetActorLocation(), GetActorLocation());
		return FMath::Abs(FRotator::NormalizeAxis(GetActorRotation().Yaw - Rotation.Yaw)) <= 45.f;
	}
	return false;
}

void ACharacterBase::ExecuteOther(ACharacterBase* ExecuteTarget, const FVector& TargetLocation, const FRotator& TargetRotation, UAnimMontage* ExecuteMontage, UAnimMontage* BeExecutedMontage)
{
	if (!HasAuthority())
	{
		ExecuteOtherToServer_Implementation(ExecuteTarget, TargetLocation, TargetRotation, ExecuteMontage, BeExecutedMontage);
	}
	ExecuteOtherToServer(ExecuteTarget, TargetLocation, TargetRotation, ExecuteMontage, BeExecutedMontage);
}

void ACharacterBase::ExecuteOtherToServer_Implementation(ACharacterBase* ExecuteTarget, const FVector& TargetLocation, const FRotator& TargetRotation, UAnimMontage* ExecuteMontage, UAnimMontage* BeExecutedMontage)
{
	ExecuteTargetCharacter = ExecuteTarget;
	ExecuteTarget->ExecuteFromCharacter = this;
	UARPG_ActorFunctionLibrary::MoveActorTo(this, TargetLocation, TargetRotation, 0.2f, false);
	PlayMontage(ExecuteMontage);
	ExecuteTarget->PlayMontage(BeExecutedMontage);
}

void ACharacterBase::WhenKillOther(ACharacterBase* WhoBeKilled, UObject* KillInstigator)
{
	Battle_Display_LOG("%s杀死了%s", *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(WhoBeKilled));
	ReceiveWhenKillOther(WhoBeKilled, KillInstigator);
	OnKillOther.Broadcast(this, WhoBeKilled, KillInstigator);
}

void ACharacterBase::WhenDamagedOther(ACharacterBase* WhoBeDamaged, float DamageValue, UObject* DamageInstigator)
{
	ensure(DamageValue > 0.f);

	Battle_Display_LOG("%s对%s造成了[%f]点伤害", *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(WhoBeDamaged), DamageValue);
	ReceiveWhenDamagedOther(WhoBeDamaged, DamageValue, DamageInstigator);
	OnDamagedOther.Broadcast(this, WhoBeDamaged, DamageValue, DamageInstigator);
}

float ACharacterBase::ApplyPointDamage(float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitInfo, class ACharacterBase* InstigatorBy, AActor* DamageCauser, TSubclassOf<class UDamageType> DamageTypeClass, const FApplyPointDamageParameter& Param)
{
	float FinalReduceValue = BaseDamage;

	if (!bCanBeDamaged || FinalReduceValue <= 0.f)
	{
		return 0.f;
	}

	if (InstigatorBy)
	{
		if (ACharacterBase* InstigatorPawn = Cast<ACharacterBase>(InstigatorBy))
		{
			//闪避
			if (Param.bCanDodge && bIsDodging)
			{
				WhenDodgeSucceed(FinalReduceValue, InstigatorBy, HitInfo);
				return 0.f;
			}
			//防御反击
			else if (Param.bCanDefenseSwipe && IsDefenseSwipeSucceed(InstigatorPawn->GetActorLocation(), HitInfo))
			{
				return 0.f;
			}
			//防御
			else if (Param.bCanDefense && IsDefenseSucceed(InstigatorPawn->GetActorLocation(), HitInfo))
			{
				WhenDefenseSucceed(FinalReduceValue, InstigatorBy, HitFromDirection, Param.DefenseBeakBackDistance, HitInfo);
				InstigatorPawn->WhenAttackedDefenseCharacter(FinalReduceValue, this, HitInfo);
				return 0.f;
			}
		}
	}

	if (!(Param.ReceiveDamageAction && Param.ReceiveDamageAction.GetDefaultObject()->PlayReceiveDamageAction(HitFromDirection, this, HitInfo, InstigatorBy, DamageCauser)))
	{
		float HitStunOverflowValue = AddHitStun(Param.AddHitStunValue);
		if (HitStunOverflowValue >= 0.f)
		{
			ReceivePlayHitStunMontage(FinalReduceValue, HitStunOverflowValue, HitFromDirection, HitInfo, InstigatorBy, DamageCauser);
			HitStunOverflowValue = 0.f;
		}
		else
		{
			ReceivePlayNormalDamageMontage(FinalReduceValue, HitFromDirection, HitInfo, InstigatorBy, DamageCauser);

			//击退效果
			FVector BeakBackOffset = HitFromDirection.GetSafeNormal2D() * Param.NormalBeakBackDistance;
			UARPG_ActorFunctionLibrary::PushActorTo(this, BeakBackOffset);
		}
	}

	UGameplayStatics::ApplyPointDamage(this, FinalReduceValue, GetActorLocation(), HitInfo, InstigatorBy ? InstigatorBy->GetController() : nullptr, DamageCauser, DamageTypeClass);
	return FinalReduceValue;
}

void ACharacterBase::InvokeInteract(AActor* InteractTarget)
{
	if (CanInteract(InteractTarget))
	{
		InvokeInteract_ToServer(InteractTarget);
	}
}

void ACharacterBase::InvokeInteract_ToServer_Implementation(AActor* InteractTarget)
{
	if (CanInteract(InteractTarget))
	{
		IARPG_InteractInterface::WhenInvokeInteract(InteractTarget, this);
	}
}

bool ACharacterBase::CanInteract(AActor* InteractTarget) const
{
	return InteractTarget && InteractTarget->Implements<UARPG_InteractInterface>() && IARPG_InteractInterface::CanInteract(InteractTarget, this);
}

