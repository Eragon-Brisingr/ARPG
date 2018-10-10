// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "XD_CharacterMovementComponent.h"
#include "ARPG_MovementComponent.h"
#include "ARPG_InventoryComponent.h"
#include "ARPG_ItemCoreBase.h"
#include <Kismet/GameplayStatics.h>
#include "XD_LevelFunctionLibrary.h"


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

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::WhenGameInit_Implementation()
{
	//初始化道具和使用中的道具
	{
		TArray<UARPG_ItemCoreBase*> RemoveItems;
		TArray<FXD_Item> FinalInitItemList = GetInitItemList();
		//对已存在的进行数量修改
		for (UARPG_ItemCoreBase* ItemCore : Inventory->GetItemCoreList())
		{
			if (const FXD_Item* Item = FinalInitItemList.FindByPredicate([ItemCore](const FXD_Item& E_Item) {return ItemCore->EqualForItemCore(E_Item.ItemCore); }))
			{
				ItemCore->Number = Item->ItemCore->Number;
				FinalInitItemList.RemoveAll([&](const FXD_Item& E) {return E->EqualForItemCore(ItemCore); });
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

TArray<struct FXD_Item> ACharacterBase::GetInitItemList() const
{
	TArray<FXD_Item> Res = ReceivedGetInitItemList();
	Res.Append(Inventory->InitItems);
	return Res;
}

void ACharacterBase::SetRebornLocation(const FVector& RebornLocation)
{
	BornLocation = RebornLocation;
	BornWorldOrigin = GetWorld()->OriginLocation;
}

FVector ACharacterBase::GetRebornLocation()
{
	return UXD_LevelFunctionLibrary::GetFixedWorldLocation(this, BornWorldOrigin, BornLocation);
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
		if (ClientMaster)
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
				PlayAnimMontage(MontageToPlay, InPlayRate, StartSectionName);
				PlayMontageToServer(MontageToPlay, InPlayRate, StartSectionName);
			}
		}
		else if (HasAuthority())
		{
			MulticastPlayMontage(MontageToPlay, InPlayRate, StartSectionName);
		}
		return MontageToPlay->GetPlayLength();
	}
	return 0.f;
}

void ACharacterBase::MulticastPlayMontage_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	PlayAnimMontage(MontageToPlay, InPlayRate, StartSectionName);
}

void ACharacterBase::MulticastPlayMontageSkipOwner_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	if (IsLocallyControlled() || GetMesh()->GetAnimInstance()->Montage_IsPlaying(MontageToPlay) == false)
	{
		PlayAnimMontage(MontageToPlay, InPlayRate, StartSectionName);
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

bool ACharacterBase::IsDefenseSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const
{
	return bIsDefense;
}

bool ACharacterBase::IsDefenseSwipeSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const
{
	return bIsDefenseSwipe;
}

