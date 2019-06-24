// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_LadderBase.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "NavLinkCustomComponent.h"
#include "ARPG_NavAreaType.h"
#include "GameFramework/Controller.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AARPG_LadderBase::AARPG_LadderBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LadderNavLinkCustomComponent = CreateDefaultSubobject<UNavLinkCustomComponent>(GET_MEMBER_NAME_CHECKED(AARPG_LadderBase, LadderNavLinkCustomComponent));
	{
		LadderNavLinkCustomComponent->SetNavigationRelevancy(true);
		LadderNavLinkCustomComponent->SetMoveReachedLink(this, &AARPG_LadderBase::NotifySmartLinkReached);
		LadderNavLinkCustomComponent->SetEnabledArea(UARPG_NavArea_LadderBase::StaticClass());
	}
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
					if (((Character->IsPlayerControlled() ? Character->GetControlRotation().Vector() : Character->GetActorForwardVector()) | MovementInputVector) > 0.f)
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

	LadderNavLinkCustomComponent->SetLinkData(FVector(-100.f, 0.f, 0.f), FVector(100.f, 0.f, LadderUnitHeight * LadderLength), ENavLinkDirection::BothWays);
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
							EnterLadderImpl(InteractInvoker, IsLowEnter);
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

	Character->bEnableFootIk = false;
	Character->bEnableAimOffset = false;
}

void AARPG_LadderBase::WhenLeaveLadder(ACharacterBase* Character)
{
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	Character->bEnableFootIk = true;
	Character->bEnableAimOffset = true;
}

void AARPG_LadderBase::EnterLadderImpl(ACharacterBase* Character, bool IsLowEnter)
{
	const FClimbLadderConfig* Config = GetClimbConfig(Character);
	int32 TargetSlot = IsLowEnter ? Config->DownEnterSlot : LadderLength - Config->UpEnterSlot;

	WhenEnterLadder(Character);
	Character->SetInteractingTarget(this);
	FInLadderData InLadderData;
	InLadderData.LadderIndex = TargetSlot;
	CharacterInLadderDatas.Add(Character, InLadderData);
	PlayLadderMontage(Character, IsLowEnter ? Config->DownEnterMontage : Config->UpEnterMontage, InLadderData.LadderIndex);
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

void AARPG_LadderBase::NotifySmartLinkReached(UNavLinkCustomComponent* LinkComp, UObject* PathingAgent, const FVector& DestPoint)
{
	if (UPathFollowingComponent* PathComp = Cast<UPathFollowingComponent>(PathingAgent))
	{
		if (AController* Controller = Cast<AController>(PathComp->GetOwner()))
		{
			if (ACharacterBase* Character = Cast<ACharacterBase>(Controller->GetPawn()))
			{
				//不能直接去交互，因为会调用寻路导至寻路被打断
				if (Character->CanInteractWithTarget(this))
				{
					float LadderHeight = LadderLength * LadderUnitHeight;
					const FClimbLadderConfig* Config = GetClimbConfig(Character);
					FVector CharacterLocation = Character->GetActorLocation();
					FTransform LowEnterLocation = Config->DownEnterLocation * GetActorTransform();
					FTransform UpEnterLocation = Config->UpEnterLocation * GetActorTransform();
					bool IsLowEnter = FMath::Abs(CharacterLocation.Z - GetActorLocation().Z) < FMath::Abs(CharacterLocation.Z - (GetActorLocation().Z + LadderHeight));
					FTransform ClambLocation = IsLowEnter ? LowEnterLocation : UpEnterLocation;
					FVector MoveLocation = ClambLocation.GetLocation();
					MoveLocation.Z = GetActorLocation().Z + (IsLowEnter ? 0.f : LadderHeight);

					UARPG_ActorMoveUtils::MoveCharacterToFitGround(Character, MoveLocation, ClambLocation.GetRotation().Rotator(),
						FOnActorMoveFinished::CreateWeakLambda(this, [=](bool bIsAborted)
							{
								if (!bIsAborted)
								{
									Character->EnterReleaseState(FOnCharacterBehaviorFinished::CreateWeakLambda(this, [=](bool Succeed)
										{
											int32 TargetSlot = IsLowEnter ? Config->DownEnterSlot : LadderLength - Config->UpEnterSlot;
											if (Succeed && CanCharacterInSlot(Character, TargetSlot))
											{
												EnterLadderImpl(Character, IsLowEnter);
											}
										}));
								}
							}));
				}
			}
		}
	}
}

