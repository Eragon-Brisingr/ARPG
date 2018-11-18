// Fill out your copyright notice in the Description page of Project Settings.

#include "SubSystem/ARPG_HatredControlSystemNormal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterBase.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"


void UARPG_HatredControlSystemNormal::WhenInitHatredControlSystem()
{
	
}

ACharacterBase* UARPG_HatredControlSystemNormal::GetMostHatredCharacter()
{
	ACharacterBase* MostHatredCharacter = nullptr;
	if (AAIController* AIController = Cast<AAIController>(Character->GetController()))
	{
		UAIPerceptionComponent* Perception = AIController->GetPerceptionComponent();
		TArray<AActor*> KnownPerceivedActors;
		Perception->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), KnownPerceivedActors);

		FAISenseID SightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());

		for (AActor* KnownPerceivedActor : KnownPerceivedActors)
		{
			if (ACharacterBase* KnowPreceivedCharacter = Cast<ACharacterBase>(KnownPerceivedActor))
			{
				if (Character->GetAttitudeTowards(KnowPreceivedCharacter) == ETeamAttitude::Hostile)
				{
					const FActorPerceptionInfo* ActorPerceptionInfo = Perception->GetActorInfo(*KnowPreceivedCharacter);

					if (const FAIStimulus* AIStimulus = ActorPerceptionInfo->LastSensedStimuli.FindByPredicate([&](const FAIStimulus& E) {return E.Type == SightSenseID; }))
					{
						if (AIStimulus->GetAge() < SightRemberAge)
						{
							if (MostHatredCharacter)
							{
								if (Character->GetDistanceTo(MostHatredCharacter) > Character->GetDistanceTo(KnowPreceivedCharacter))
								{
									MostHatredCharacter = KnowPreceivedCharacter;
								}
							}
							else
							{
								MostHatredCharacter = KnowPreceivedCharacter;
							}
						}
					}
				}
			}
		}
	}
	return MostHatredCharacter;
}

void UARPG_HatredControlSystemNormal::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,AController* EventInstigator, AActor* DamageCauser)
{
	
}
