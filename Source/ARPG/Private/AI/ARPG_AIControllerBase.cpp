// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AIControllerBase.h"
#include "ARPG_AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "CharacterBase.h"
#include "ARPG_PathFollowingComponent.h"

AARPG_AIControllerBase::AARPG_AIControllerBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_PathFollowingComponent>(TEXT("PathFollowingComponent")))
{
	AIPerception = CreateDefaultSubobject<UARPG_AIPerceptionComponent>(GET_MEMBER_NAME_CHECKED(AARPG_AIControllerBase, AIPerception));
	{
		UAISenseConfig_Sight* Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight_Config"));
		Sight->SightRadius = 4000.f;
		Sight->LoseSightRadius = 5000.f;
		Sight->PeripheralVisionAngleDegrees = 75.f;
		Sight->SetMaxAge(20.f);
		Sight->AutoSuccessRangeFromLastSeenLocation = 100.f;
		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectNeutrals = true;
		Sight->DetectionByAffiliation.bDetectFriendlies = true;

		AIPerception->ConfigureSense(*Sight);

		UAISenseConfig_Hearing* Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing_Config"));
		Hearing->HearingRange = 2000.f;
		Hearing->SetMaxAge(2.f);
		Hearing->DetectionByAffiliation.bDetectEnemies = true;
		Hearing->DetectionByAffiliation.bDetectNeutrals = true;
		Hearing->DetectionByAffiliation.bDetectFriendlies = true;

		AIPerception->ConfigureSense(*Hearing);
	}
}

void AARPG_AIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacterBase* ControlledCharacter = Cast<ACharacterBase>(GetPawn()))
	{
		RunBehaviorTree(ControlledCharacter->MainBehaviorTree);
	}
}

ETeamAttitude::Type AARPG_AIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (ACharacterBase* ControlledCharacter = Cast<ACharacterBase>(GetPawn()))
	{
		ControlledCharacter->GetAttitudeTowards(&Other);
	}
	return ETeamAttitude::Neutral;
}
