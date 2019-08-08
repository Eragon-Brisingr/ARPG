// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_HUDBase.h"
#include <Components/SphereComponent.h>
#include <GameFramework/PlayerController.h>
#include "ARPG_InteractInterface.h"
#include "CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ARPG_CollisionType.h"


AARPG_HUDBase::AARPG_HUDBase()
{

}

void AARPG_HUDBase::BeginPlay()
{
	Super::BeginPlay();

	EnableInput(GetOwningPlayerController());
}

void AARPG_HUDBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ACharacterBase * Character = Cast<ACharacterBase>(GetOwningPlayerController()->GetPawn()))
	{
		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Character->GetActorLocation(), Character->GetActorLocation() + FVector::UpVector, MaxInteractTraceRadius, FARPG_TraceQueryType::Visibility, false, { Character }, EDrawDebugTrace::None, HitResults, false);

		TArray<AActor*> PotentialInteractableActors;
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* OtherActor = HitResult.GetActor();
			if (OtherActor && OtherActor->Implements<UARPG_InteractInterface>() && IARPG_InteractInterface::CanShowHintInfo(OtherActor, Character) && !PotentialInteractableActors.Contains(OtherActor))
			{
				PotentialInteractableActors.Add(OtherActor);
			}
		}

		Algo::Sort(PotentialInteractableActors, [&](AActor* LHS, AActor* RHS)
			{
				if (!Character->CanInteractWithTarget(RHS))
				{
					return true;
				}
				return Character->GetHorizontalDistanceTo(LHS) < Character->GetHorizontalDistanceTo(RHS);
			});

		TSet<AActor*> PreShowHintActors(ShowHintActors);
		ShowHintActors.Empty();
		for (AActor* PotentialInteractableActor : PotentialInteractableActors)
		{
			ShowHintActors.Add(PotentialInteractableActor);
		}

		for (AActor* EnableInteractActor : TSet<AActor*>(ShowHintActors).Difference(PreShowHintActors))
		{
			OnActorEnableHint.Broadcast(EnableInteractActor);
		}
		for (AActor* DisableInteractActor : PreShowHintActors.Difference(TSet<AActor*>(ShowHintActors)))
		{
			OnActorDisableHint.Broadcast(DisableInteractActor);
		}
	}
}

AActor* AARPG_HUDBase::GetNearestShowHintActor() const
{
	if (ShowHintActors.Num() > 0)
	{
		return ShowHintActors[0];
	}
	return nullptr;
}
