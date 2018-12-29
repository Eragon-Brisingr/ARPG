// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SubSystem/ARPG_HatredControlSystemBase.h"
#include "ARPG_HatredControlSystemNormal.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_HatredControlSystemNormal : public UARPG_HatredControlSystemBase
{
	GENERATED_BODY()
public:
	virtual void WhenInitHatredControlSystem() override;

	virtual class ACharacterBase* GetMostHatredCharacter(const TArray<class ACharacterBase*>& Sources) override;

	virtual void TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, class  AActor* DamageCauser) override;
};
