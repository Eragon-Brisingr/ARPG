// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include <Engine/World.h>
#include <Animation/AnimInstance.h>
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>
#include <Kismet/KismetMathLibrary.h>
#include <Perception/AISenseConfig_Sight.h>

#include "ARPG_MovementComponent.h"
#include "ARPG_InventoryComponent.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_Battle_Log.h"
#include "ReceiveDamageActionBase.h"
#include "ARPG_DebugFunctionLibrary.h"
#include "ARPG_LevelFunctionLibrary.h"
#include "XD_TemplateLibrary.h"
#include "UnrealNetwork.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "ARPG_CampInfo.h"
#include "ARPG_CampRelationship.h"
#include "ARPG_HatredControlSystemNormal.h"
#include "ARPG_BattleStyleSystemNormal.h"
#include "ARPG_AlertSystemNormal.h"
#include "ARPG_SneakSystemNormal.h"
#include "ARPG_NavigationQueryFilter.h"
#include "ARPG_CharacterTurnBase.h"
#include "Engine/Engine.h"
#include "ARPG_EnterReleaseStateBase.h"
#include "ARPG_PlayerControllerBase.h"
#include "XD_DispatchableActionBase.h"
#include "ARPG_AD_CharacterInteract.h"


// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_MovementComponent>(ACharacter::CharacterMovementComponentName))
	, bEnableAimOffset(true)
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

	HatredControlSystem = CreateDefaultSubobject<UARPG_HatredControlSystemNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, HatredControlSystem));

	BattleStyleSystem = CreateDefaultSubobject<UARPG_BattleStyleSystemNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, BattleStyleSystem));

	AlertSystem = CreateDefaultSubobject<UARPG_AlertSystemNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, AlertSystem));

	SneakSystem = CreateDefaultSubobject<UARPG_SneakSystemNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, SneakSystem));

	CharacterTurnAction = CreateDefaultSubobject<UCA_TurnNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, CharacterTurnAction));

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	NavigationQueryFilter = UARPG_NavigationQueryFilter::StaticClass();
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (InteractBehavior)
	{
		InteractBehavior->Character = this;
		InteractBehavior->InitLeader(this);
	}
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

bool ACharacterBase::NeedSave_Implementation() const
{
	return IsPlayerControlled() == false;
}

void ACharacterBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	//先于组件的初始化注册，使得组件中可以覆盖注册
	BattleControl = this;
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO 挪入bIsLockingOther与bInvokeSprint的Setter驱动而不是轮询
	if (IsPlayerControlled())
	{
		ARPG_MovementComponent->SetRotationMode(bIsLockingOther && ARPG_MovementComponent->bInvokeSprint == false ? ECharacterRotationMode::LookingDirection : ECharacterRotationMode::VelocityDirection);
	}
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
	DOREPLIFETIME_CONDITION(ACharacterBase, bIsInteractingWithActor, COND_OwnerOnly);
}

void ACharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (HatredControlSystem)
	{
		HatredControlSystem->InitHatredControlSystem(this);
	}

	if (BattleStyleSystem)
	{
		BattleStyleSystem->InitBattleStyleSystem(this);
	}

	if (AlertSystem)
	{
		AlertSystem->InitAlertSystem(this);
	}

	if (SneakSystem)
	{
		SneakSystem->InitSneakSystem(this);
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,	AActor* DamageCauser)
{
	if (HatredControlSystem)
	{
		HatredControlSystem->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
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
			if (const FARPG_Item* Item = FinalInitItemList.FindByPredicate([ItemCore](const FARPG_Item& E_Item) {return ItemCore->IsEqualWithItemCore(E_Item.ItemCore); }))
			{
				ItemCore->Number = Item->ItemCore->Number;
				FinalInitItemList.RemoveAll([&](const FARPG_Item& E) {return E->IsEqualWithItemCore(ItemCore); });
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

UXD_DispatchableActionBase* ACharacterBase::GetCurrentDispatchableAction_Implementation() const
{
	return CurrentAction.Get();
}

void ACharacterBase::SetCurrentDispatchableAction_Implementation(UXD_DispatchableActionBase* Action)
{
	CurrentAction = Action;
}

UARPG_ActionDispatcherBase* ACharacterBase::GetCurrentDispatcher_Implementation() const
{
	return CurrentDispatcher;
}

void ACharacterBase::SetCurrentDispatcher_Implementation(UXD_ActionDispatcherBase* Dispatcher)
{
	CurrentDispatcher = Dispatcher ? CastChecked<UARPG_ActionDispatcherBase>(Dispatcher) : nullptr;
}

bool ACharacterBase::CanExecuteDispatchableAction_Implementation() const
{
	return AlertValue == 0.f;
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

void ACharacterBase::StopMovement()
{
	if (Controller)
	{
		Controller->StopMovement();
	}
	if (CharacterTurnAction && CharacterTurnAction->bIsExecuting)
	{
		CharacterTurnAction->AbortTurnTo(this, {});
	}
	UARPG_ActorMoveUtils::StopActorMove(this);
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

void ACharacterBase::StopMontage_Implementation(UAnimMontage* MontageToStop)
{
	StopAnimMontage(MontageToStop);
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
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		return AnimInstance->GetSlotMontageGlobalWeight(FullBodySlotName) == 0.f;
	}
	return false;
}

void ACharacterBase::EnableAutoUpdateLookAtRotation(bool Enable)
{
	ARPG_MovementComponent->bAutoUpdateLookingRotation = Enable;
}

void ACharacterBase::SetLookAtRotation(const FRotator& LookAtRotation)
{
	ARPG_MovementComponent->LookingRotation = LookAtRotation;
}

void ACharacterBase::SetLookAtLocation(const FVector& WorldLocation)
{
	ARPG_MovementComponent->LookingRotation = (WorldLocation - GetPawnViewLocation()).Rotation();
}

FRotator ACharacterBase::GetLookAtRotation() const
{
	return ARPG_MovementComponent->LookingRotation;
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

UARPG_CharacterBehaviorBase* ACharacterBase::EnterReleaseState(const FOnCharacterBehaviorFinished& OnBehaviorFinished)
{
	if (EnterReleaseStateAction && IsInReleaseState() == false)
	{
		EnterReleaseStateAction->EnterReleaseState(this, OnBehaviorFinished);
		return EnterReleaseStateAction;
	}
	else
	{
		OnBehaviorFinished.ExecuteIfBound(true);
		return nullptr;
	}
}

bool ACharacterBase::CanTurnTo() const
{
	return GetMesh()->GetAnimInstance()->GetSlotMontageGlobalWeight(TurnSlotName) < 1.f && IsPlayingRootMotion() == false;
}

bool ACharacterBase::TurnTo(const FRotator& TargetWorldRotation, const FOnCharacterBehaviorFinished& OnCharacterBehaviorFinished)
{
	if (CharacterTurnAction)
	{
		if (CanTurnTo())
		{
			CharacterTurnAction->TurnTo(this, TargetWorldRotation, OnCharacterBehaviorFinished);
			return true;
		}
	}
	else
	{
		UARPG_ActorMoveUtils::MoveCharacterToRotationFitGround(this, TargetWorldRotation);
		OnCharacterBehaviorFinished.ExecuteIfBound(true);
		return true;
	}
	return false;
}

bool ACharacterBase::TurnTo(const FRotator& TargetWorldRotation)
{
	return TurnTo(TargetWorldRotation, {});
}

void ACharacterBase::ForceSetClientWorldLocation(const FVector& Location)
{
	ForceSetClientWorldLocationImpl(FRepMovement::RebaseOntoZeroOrigin(Location, this));
}

void ACharacterBase::ForceSetClientWorldLocationAndRotation(const FVector& Location, const FRotator& Rotation)
{
	ForceSetClientWorldLocationAndRotationImpl(FRepMovement::RebaseOntoZeroOrigin(Location, this), Rotation);
}

void ACharacterBase::ForceSetClientWorldLocationImpl_Implementation(const FVector& Location)
{
	SetActorLocation(FRepMovement::RebaseOntoLocalOrigin(Location, this));
}

bool ACharacterBase::ForceSetClientWorldLocationImpl_Validate(const FVector& Location)
{
	return true;
}

void ACharacterBase::ForceSetClientWorldRotation_Implementation(const FRotator& Rotation)
{
	SetActorRotation(Rotation);
}

bool ACharacterBase::ForceSetClientWorldRotation_Validate(const FRotator& Rotation)
{
	return true;
}

void ACharacterBase::ForceSetClientWorldLocationAndRotationImpl_Implementation(const FVector& Location, const FRotator& Rotation)
{
	SetActorLocationAndRotation(FRepMovement::RebaseOntoLocalOrigin(Location, this), Rotation);
}

bool ACharacterBase::ForceSetClientWorldLocationAndRotationImpl_Validate(const FVector& Location, const FRotator& Rotation)
{
	return true;
}

void ACharacterBase::PlayMontageWithBlendingOutDelegate(UAnimMontage* Montage, const FOnMontageBlendingOutStarted& OnMontageBlendingOutStarted, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	PlayMontage(Montage, InPlayRate, StartSectionName);
	GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(const_cast<FOnMontageBlendingOutStarted&>(OnMontageBlendingOutStarted), Montage);
}

void ACharacterBase::ClearMontageBlendingOutDelegate(UAnimMontage* Montage)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(Montage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
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
		if (E_ItemCore->IsEqualWithItemCore(ItemCore))
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
	UARPG_ActorMoveUtils::PushActorTo(this, BeakBackOffset);
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
	if (ExecuteTarget)
	{
		ExecuteTargetCharacter = ExecuteTarget;
		ExecuteTarget->ExecuteFromCharacter = this;
		UARPG_ActorMoveUtils::MoveActorTo(ExecuteTarget, TargetLocation, TargetRotation);
		PlayMontage(ExecuteMontage);
		ExecuteTarget->PlayMontage(BeExecutedMontage);
	}
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
			UARPG_ActorMoveUtils::PushActorTo(this, BeakBackOffset);
		}
	}

	UGameplayStatics::ApplyPointDamage(this, FinalReduceValue, GetActorLocation(), HitInfo, InstigatorBy ? InstigatorBy->GetController() : nullptr, DamageCauser, DamageTypeClass);
	return FinalReduceValue;
}

bool ACharacterBase::IsSneaking() const
{
	return GetCharacterMovement()->IsCrouching();
}

void ACharacterBase::InvokeInteract(AActor* InteractTarget)
{
	if (CanInteractWithTarget(InteractTarget))
	{
		InvokeInteract_ToServer(InteractTarget);
	}
}

void ACharacterBase::InvokeInteract_ToServer_Implementation(AActor* InteractTarget)
{
	if (CanInteractWithTarget(InteractTarget))
	{
		InvokeInteractWithEndEvent(InteractTarget, {});
	}
}

bool ACharacterBase::InvokeInteract_ToServer_Validate(AActor* InteractTarget)
{
	return true;
}

void ACharacterBase::InvokeInteractWithEndEvent(AActor* InteractTarget, const FOnInteractEnd& OnInteractEndEvent)
{
	check(HasAuthority() && InteractTarget);
#if WITH_EDITOR
	checkf(PreInvokeInteractTarget.IsValid() == false, TEXT("PreInvokeInteractTarget 还存在，上一个交互未调用ExecuteInteractEnd或ExecuteInteractAbortEnd"));
	PreInvokeInteractTarget = InteractTarget;
#endif
	InvokeInteractTarget = InteractTarget;
	OnInteractEnd = OnInteractEndEvent;
	IARPG_InteractInterface::WhenInvokeInteract(InteractTarget, this);
}

bool ACharacterBase::CanInteractWithTarget(AActor* InteractTarget) const
{
	return InvokeInteractTarget == nullptr && InteractTarget && CanPlayFullBodyMontage() && InteractTarget->Implements<UARPG_InteractInterface>() && IARPG_InteractInterface::CanInteract(InteractTarget, this);
}

void ACharacterBase::InvokeAbortInteract()
{
	if (InvokeInteractTarget)
	{
		InvokeAbortInteract_ToServer();
	}
}

void ACharacterBase::InvokeAbortInteractWithAbortEvent(const FOnInteractAbortEnd& OnInteractAbortEndEvent)
{
	check(HasAuthority());

	OnInteractAbortEnd = OnInteractAbortEndEvent;
	IARPG_InteractInterface::WhenAbortInteract(InvokeInteractTarget, this);
}

void ACharacterBase::InvokeAbortInteract_ToServer_Implementation()
{
	if (InvokeInteractTarget)
	{
		InvokeAbortInteractWithAbortEvent({});
	}
}

bool ACharacterBase::InvokeAbortInteract_ToServer_Validate()
{
	return true;
}

void ACharacterBase::InvokeFinishPathFollowing_ToServer_Implementation()
{
	GetController()->StopMovement();
}

bool ACharacterBase::InvokeFinishPathFollowing_ToServer_Validate()
{
	return true;
}

void ACharacterBase::WhenExecuteInteract_Implementation(ACharacterBase* InteractInvoker)
{
	//TODO 呼叫InteractEndEvent
	if (OnPreInteractEvents.Num() > 0)
	{
		for (FOnPreInteractEvent& OnPreInteractEvent : OnPreInteractEvents)
		{
			if (OnPreInteractEvent.IsBound())
			{
				OnPreInteractEvent.Execute();
				break;
			}
		}
	}
	else if (InteractBehavior)
	{
		InteractBehavior->InteractTarget = InteractInvoker;
		if (InteractBehavior->CanStartDispatcher())
		{
			InteractBehavior->WhenDispatchFinishedNative = FWhenDispatchFinishedNative::CreateWeakLambda(InteractInvoker, [InteractInvoker](const FName & Tag)
				{
					InteractInvoker->ExecuteInteractEnd(EInteractEndResult::Succeed);
				});
			InteractBehavior->OnDispatcherAbortedNative = FOnDispatcherAbortedNative::CreateWeakLambda(InteractInvoker, [InteractInvoker]()
				{
					InteractInvoker->ExecuteInteractAbortEnd();
				});
			InteractBehavior->StartDispatch();
		}
	}
}

bool ACharacterBase::CanInteract_Implementation(const class ACharacterBase* InteractInvoker) const
{
	if (InteractInvoker == this)
	{
		return false;
	}
	if (CurrentDispatcher && CurrentDispatcher->bInteractable == false && CurrentDispatcher->State != EActionDispatcherState::Deactive)
	{
		return false;
	}

	return true;
}

void ACharacterBase::ExecuteInteractEnd(EInteractEndResult Result)
{
#if WITH_EDITOR
	PreInvokeInteractTarget = nullptr;
#endif
	InvokeInteractTarget = nullptr;
	if (OnInteractEnd.IsBound())
	{
		OnInteractEnd.Execute(Result);
		OnInteractEnd.Unbind();
	}
}

void ACharacterBase::ExecuteInteractAbortEnd()
{
#if WITH_EDITOR
	PreInvokeInteractTarget = nullptr;
#endif
	InvokeInteractTarget = nullptr;
	if (OnInteractAbortEnd.IsBound())
	{
		OnInteractAbortEnd.Execute();
		OnInteractAbortEnd.Unbind();
	}
}

class UARPG_CampInfo* ACharacterBase::GetCampInfo() const
{
	return Cast<UARPG_CampInfo>(CampConfig.GetCamp(this));
}

class UARPG_CampRelationship* ACharacterBase::GetCampRelationshipToward(ACharacterBase* Other) const
{
	return Other ? Cast<UARPG_CampRelationship>(GetCampInfo()->GetCampRelationshipRef(this, Other->GetCampInfo())) : nullptr;
}

ETeamAttitude::Type ACharacterBase::GetRelationshipTowards(const AActor* Actor) const
{
	if (const ACharacterBase* Character = Cast<ACharacterBase>(Actor))
	{
		EXD_CampRelationship CampRelationship = GetCampInfo()->GetCampRelationship(this, Character->GetCampInfo());
		switch (CampRelationship)
		{
		case EXD_CampRelationship::SelfCamp:
			return ETeamAttitude::Friendly;
		case EXD_CampRelationship::Friend:
			return ETeamAttitude::Friendly;
		case EXD_CampRelationship::Neutral:
			return ETeamAttitude::Neutral;
		case EXD_CampRelationship::Hostile:
			return ETeamAttitude::Hostile;
		}
	}
	return ETeamAttitude::Neutral;
}

ECharacterRelationship ACharacterBase::GetRelationshipTowards(const ACharacterBase* Towards) const
{
	float CampRelationShip = GetCampInfo()->GetCampRelationshipValue(this, Towards->GetCampInfo());

	//TODO : 添加角色关系
	float FinalRelationShip = CampRelationShip;
	if (FinalRelationShip > 75.f)
	{
		return ECharacterRelationship::Cordial;
	}
	else if (FinalRelationShip > 25.f)
	{
		return ECharacterRelationship::Friendly;
	}
	else if (FinalRelationShip > -25.f)
	{
		return ECharacterRelationship::Neutral;
	}
	else if (FinalRelationShip > -75.f)
	{
		return ECharacterRelationship::Hate;
	}
	else
	{
		return ECharacterRelationship::Hostile;
	}
}

void ACharacterBase::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	GetMesh()->GetSocketWorldLocationAndRotation(HeadSocketName, out_Location, out_Rotation);
}

bool ACharacterBase::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor /*= NULL*/) const
{
	if (const ACharacterBase* SightListener = Cast<ACharacterBase>(IgnoreActor))
	{
		// we need to do tests ourselves
		FHitResult HitResult;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, GetActorLocation()
			, SightCollisionChannel
			, FCollisionQueryParams(SCENE_QUERY_STAT(AILineOfSight), true, SightListener));

		if (HitResult.GetActor() == this)
		{
			float SightVigilanceValue = SightListener->AlertSystem->GetSightAddAlertValue(ObserverLocation, HitResult.ImpactPoint, this);
			if (SightVigilanceValue > 0.f)
			{
				NumberOfLoSChecksPerformed += 1;
				OutSightStrength = SightVigilanceValue;
				OutSeenLocation = HitResult.ImpactPoint;
				return true;
			}
		}
	}

	return false;
}

void ACharacterBase::WhenReceivedMoveRequest()
{

}

void ACharacterBase::AddAlertValue(float AddValue)
{
	if (AddValue > 0.f)
	{
		if (AlertValue < AlertEntirelyValue)
		{
			if (AlertValue == 0.f)
			{
				BattleStyleSystem->WhenEnterAlertState();
			}

			AlertValue += AddValue;
			if (AlertValue >= AlertEntirelyValue)
			{
				AlertValue = AlertEntirelyValue;
				BattleStyleSystem->WhenEnterBattleState();

				GetWorld()->GetTimerManager().SetTimer(AlertSubsided_TimerHandle, this, &ACharacterBase::AlertSubsided, 1.f, true, AlertEntirelySubsidedDelay);
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(AlertSubsided_TimerHandle, this, &ACharacterBase::AlertSubsided, 1.f, true, AlertNotEntirelySubsidedDelay);
			}
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(AlertSubsided_TimerHandle, this, &ACharacterBase::AlertSubsided, 1.f, true, AlertNotEntirelySubsidedDelay);
		}
	}
}

void ACharacterBase::ReduceAlertValue(float ReduceValue)
{
	if (ReduceValue > 0.f && AlertValue > 0.f)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (AlertValue == AlertEntirelyValue)
		{
			BattleStyleSystem->WhenLeaveBattleState();
		}

		AlertValue -= ReduceValue;
		if (AlertValue <= 0.f)
		{
			AlertValue = 0.f;
			BattleStyleSystem->WhenLeaveAlertState();

			GetWorld()->GetTimerManager().ClearTimer(AlertSubsided_TimerHandle);
		}
	}
}

void ACharacterBase::AlertSubsided()
{
	ReduceAlertValue(AlertSubsidedSpeed);
}

EAlertState ACharacterBase::GetAlertState() const
{
	if (AlertValue == 0.f)
	{
		return EAlertState::None;
	}
	else if (AlertValue == AlertEntirelyValue)
	{
		return EAlertState::AlertEntirely;
	}
	else
	{
		return EAlertState::Alerting;
	}
}

bool ACharacterBase::CanInDailyBehavior() const
{
	return GetAlertState() == EAlertState::None && (CurrentAction.IsValid() == false || CurrentAction->State == EDispatchableActionState::Deactive || bIsInBTreeInteracting);
}
