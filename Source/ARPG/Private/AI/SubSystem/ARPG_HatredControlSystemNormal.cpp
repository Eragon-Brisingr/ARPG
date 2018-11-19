// Fill out your copyright notice in the Description page of Project Settings.

#include "SubSystem/ARPG_HatredControlSystemNormal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterBase.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "AIPerceptionExLibrary.h"


void UARPG_HatredControlSystemNormal::WhenInitHatredControlSystem()
{
	
}

ACharacterBase* UARPG_HatredControlSystemNormal::GetMostHatredCharacter()
{
	ACharacterBase* MostHatredCharacter = nullptr;
	if (AAIController* AIController = Cast<AAIController>(Character->GetController()))
	{
		UAIPerceptionComponent* Perception = AIController->GetPerceptionComponent();

		TArray<ACharacterBase*> KnownPerceivedCharacters = UAIPerceptionExLibrary::FilterPerceivedActorsByMaxAge(Perception, UAIPerceptionExLibrary::GetKnownPerceivedActorsEx<ACharacterBase>(Perception, UAISense_Sight::StaticClass()), UAISense_Sight::StaticClass(), 5.f);
		for (ACharacterBase* KnowPreceivedCharacter : KnownPerceivedCharacters)
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
	return MostHatredCharacter;
}

void UARPG_HatredControlSystemNormal::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,AController* EventInstigator, AActor* DamageCauser)
{
	
}
