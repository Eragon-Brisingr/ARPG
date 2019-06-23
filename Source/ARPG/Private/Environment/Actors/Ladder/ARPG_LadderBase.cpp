// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_LadderBase.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARPG_ActorFunctionLibrary.h"

// Sets default values
AARPG_LadderBase::AARPG_LadderBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AARPG_LadderBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARPG_LadderBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
	{
		return;
	}

	for (const auto& Data : CharacterInLadderDatas)
	{
		if (ACharacterBase * Character = Data.Key)
		{
			FVector MovementInputVector = Character->GetMovementInputVector();
			if (!MovementInputVector.IsNearlyZero())
			{
				const FClimbLadderConfig* Config = GetClimbConfig(Character);
				if (Character->GetCurrentMontage() == Config->IdleLoopMontage)
				{
					const FInLadderData& InLadderData = Data.Value;
					if ((Character->GetControlRotation().Vector() | MovementInputVector) > 0.f)
					{
						int32 TargetSlot = InLadderData.LadderIndex + Config->ClimbUpSlotNumber;
						if (CanCharacterInSlot(Character, TargetSlot))
						{
							if (TargetSlot > LadderLength - Config->UpLeaveSlot)
							{
								Character->PlayMontageWithBlendingOutDelegate(Config->UpLeaveMontage, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=](class UAnimMontage* Montage, bool bInterrupted)
									{
										Character->ExecuteInteractEnd(EInteractEndResult::Succeed);
										CharacterInLadderDatas.Remove(Character);
									}), FARPG_MontagePlayConfig());
							}
							else
							{
								PlayLadderMontage(Character, Config->UpLoopMontage, TargetSlot);
							}
						}
					}
					else
					{
						int32 TargetSlot = InLadderData.LadderIndex - Config->ClimbDownSlotNumber;
						if (CanCharacterInSlot(Character, TargetSlot))
						{
							if (TargetSlot < Config->DownLeaveSlot)
							{
								Character->PlayMontageWithBlendingOutDelegate(Config->DownLeaveMontage, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=](class UAnimMontage* Montage, bool bInterrupted)
									{
										Character->ExecuteInteractEnd(EInteractEndResult::Succeed);
										CharacterInLadderDatas.Remove(Character);
									}), FARPG_MontagePlayConfig());
							}
							else
							{
								PlayLadderMontage(Character, Config->DownLoopMontage, TargetSlot);
							}
						}
					}
				}
			}
		}
	}
}

void AARPG_LadderBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void AARPG_LadderBase::WhenInvokeInteract_Implementation(ACharacterBase* InteractInvoker)
{
	float LadderHeight = LadderLength * LadderUnitHeight;
	const FClimbLadderConfig* Config = GetClimbConfig(InteractInvoker);
	FVector CharacterLocation = InteractInvoker->GetActorLocation();
	FTransform LowEnterLocation = Config->DownEnterLocation * GetActorTransform();
	FTransform UpEnterLocation = Config->UpEnterLocation * GetActorTransform();
	bool IsLowEnter = FMath::Abs(CharacterLocation.Z - GetActorLocation().Z) < FMath::Abs(CharacterLocation.Z - (GetActorLocation().Z + LadderHeight));
	FTransform ClambLocation = IsLowEnter ? LowEnterLocation : UpEnterLocation;
	FVector MoveLocation = ClambLocation.GetLocation();
	MoveLocation.Z = GetActorLocation().Z + (IsLowEnter ? 0.f : LadderHeight);
	UARPG_MoveUtils::ARPG_MoveToLocationAndTurn(InteractInvoker, MoveLocation, ClambLocation.GetRotation().Rotator(), FOnARPG_MoveFinished::CreateWeakLambda(this, [=](const FPathFollowingResult& Result)
		{
			if (Result.Code == EPathFollowingResult::Success)
			{
				InteractInvoker->EnterReleaseState(FOnCharacterBehaviorFinished::CreateWeakLambda(this, [=](bool Succeed)
					{
						int32 TargetSlot = IsLowEnter ? Config->DownEnterSlot : LadderLength - Config->UpEnterSlot;
						if (Succeed && CanCharacterInSlot(InteractInvoker, TargetSlot))
						{
							WhenEnterLadder(InteractInvoker);
							InteractInvoker->SetInteractingTarget(this);
							FInLadderData InLadderData;
							InLadderData.LadderIndex = TargetSlot;
							CharacterInLadderDatas.Add(InteractInvoker, InLadderData);
							PlayLadderMontage(InteractInvoker, IsLowEnter ? Config->DownEnterMontage : Config->UpEnterMontage, InLadderData.LadderIndex);
						}
						else
						{
							InteractInvoker->ExecuteInteractEnd(EInteractEndResult::CanNotInteract);
						}
					}));
			}
			else
			{
				InteractInvoker->ExecuteInteractEnd(EInteractEndResult::CanNotInteract);
			}
		}), 0.f);
}

bool AARPG_LadderBase::CanInteract_Implementation(const ACharacterBase* InteractInvoker) const
{
	if (const FClimbLadderConfig* Config = GetClimbConfig(InteractInvoker))
	{
		FTransform LowEnterLocation = Config->DownEnterLocation * GetActorTransform();
		FTransform UpEnterLocation = Config->UpEnterLocation * GetActorTransform();

		FVector CharacterLocation = InteractInvoker->GetActorLocation();
		bool IsLowEnter = FMath::Abs(CharacterLocation.Z - GetActorLocation().Z) < FMath::Abs(CharacterLocation.Z - GetActorLocation().Z + LadderLength * LadderUnitHeight);

		return IsLowEnter ? CanDownEnter(InteractInvoker) : CanUpEnter(InteractInvoker);
	}
	return false;
}

void AARPG_LadderBase::WhenBeginInteract_Implementation(ACharacterBase* InteractInvoker)
{
	WhenEnterLadder(InteractInvoker);
}

void AARPG_LadderBase::WhenEndInteract_Implementation(ACharacterBase* InteractInvoker)
{
	WhenLeaveLadder(InteractInvoker);
}

int32 AARPG_LadderBase::GetCharacterHeightUnit(const ACharacterBase* Character) const
{
	return FMath::CeilToInt(Character->GetSimpleCollisionHalfHeight() * 2.f / LadderUnitHeight);
}

bool AARPG_LadderBase::CanCharacterInSlot(const ACharacterBase* Character, int32 Slot) const
{
	for (const auto& Data : CharacterInLadderDatas)
	{
		if (Data.Key != Character)
		{
			const FInLadderData& InLadderData = Data.Value;
			if (!(InLadderData.LadderIndex >= Slot + GetCharacterHeightUnit(Character) || InLadderData.LadderIndex + GetCharacterHeightUnit(Data.Key) <= Slot))
			{
				return false;
			}
		}
	}
	return true;
}

FVector AARPG_LadderBase::GetSlotWorldLocation(const ACharacterBase* Character, int32 Slot) const
{
	FVector WorldLocation = Character->GetActorLocation();
	WorldLocation.Z = GetActorLocation().Z + Slot * LadderUnitHeight;
	return WorldLocation;
}

void AARPG_LadderBase::PlayLadderMontage(ACharacterBase* Character, UAnimMontage* Montage, int32 NextSlot)
{
	CharacterInLadderDatas.Find(Character)->LadderIndex = NextSlot;
	Character->PlayMontageWithBlendingOutDelegate(Montage, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=](class UAnimMontage* Montage, bool bInterrupted)
		{
			if (!bInterrupted)
			{
				const FClimbLadderConfig* Config = GetClimbConfig(Character);
				FVector ClimbLocation = GetActorTransform().TransformPosition(GetClimbConfig(Character)->ClimbingLocation);
				ClimbLocation.Z = GetActorLocation().Z + LadderUnitHeight * NextSlot + Character->GetSimpleCollisionHalfHeight();
				UARPG_ActorMoveUtils::MoveActorTo(Character, ClimbLocation, GetActorRotation());
				Character->PlayMontage(Config->IdleLoopMontage, FARPG_MontagePlayConfig());
			}
		}), FARPG_MontagePlayConfig());
}

void AARPG_LadderBase::WhenEnterLadder(ACharacterBase* Character)
{
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	//关闭FootIk
	Character->GetMesh()->SetDisablePostProcessBlueprint(true);
}

void AARPG_LadderBase::WhenLeaveLadder(ACharacterBase* Character)
{
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	//关闭FootIk
	Character->GetMesh()->SetDisablePostProcessBlueprint(false);
}

bool AARPG_LadderBase::CanDownEnter(const ACharacterBase* Character) const
{
	const FClimbLadderConfig* ClimbLadderConfig = GetClimbConfig(Character);
	return CanCharacterInSlot(Character, ClimbLadderConfig->DownEnterSlot);
}

bool AARPG_LadderBase::CanUpEnter(const ACharacterBase* Character) const
{
	const FClimbLadderConfig* ClimbLadderConfig = GetClimbConfig(Character);
	return CanCharacterInSlot(Character, ClimbLadderConfig->UpEnterSlot);
}

const FClimbLadderConfig* AARPG_LadderBase::GetClimbConfig(const ACharacterBase* Character) const
{
	for (UClass* Class = Character->GetClass(); Class != ACharacterBase::StaticClass(); Class = Class->GetSuperClass())
	{
		if (const FClimbLadderConfig* ClimbLadderConfig = ClimbLadderConfigs.Find(Class))
		{
			return ClimbLadderConfig;
		}
	}
	return nullptr;
}

