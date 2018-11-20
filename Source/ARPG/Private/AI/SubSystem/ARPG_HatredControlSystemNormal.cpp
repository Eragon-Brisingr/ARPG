// Fill out your copyright notice in the Description page of Project Settings.

#include "SubSystem/ARPG_HatredControlSystemNormal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterBase.h"
#include "AIController.h"


void UARPG_HatredControlSystemNormal::WhenInitHatredControlSystem()
{
	
}

ACharacterBase* UARPG_HatredControlSystemNormal::GetMostHatredCharacter(const TArray<class ACharacterBase*>& Sources)
{
	ACharacterBase* MostHatredCharacter = nullptr;
	for (ACharacterBase* KnowPreceivedCharacter : Sources)
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
	return MostHatredCharacter;
}

void UARPG_HatredControlSystemNormal::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,AController* EventInstigator, AActor* DamageCauser)
{
	
}
