// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "ARPG_HatredControlSystemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, collapsecategories, abstract)
class ARPG_API UARPG_HatredControlSystemBase : public UObject
{
	GENERATED_BODY()
public:
	virtual class UWorld* GetWorld() const override;

	UPROPERTY(BlueprintReadOnly, Category = "仇恨系统")
	class ACharacterBase* Character;

	void InitHatredControlSystem(class ACharacterBase* Owner)
	{
		Character = Owner;
		WhenInitHatredControlSystem();
	}
	virtual void WhenInitHatredControlSystem() { ReceiveWhenInitHatredControlSystem(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "仇恨系统", meta = (DisplayName = "WhenInitHatredControlSystem"))
	void ReceiveWhenInitHatredControlSystem();

	UFUNCTION(BlueprintCallable, Category = "仇恨系统")
	virtual class ACharacterBase* GetMostHatredCharacter(const TArray<class ACharacterBase*>& Sources) { return ReceiveGetMostHatredCharacter(Sources); }
	UFUNCTION(BlueprintImplementableEvent, Category = "仇恨系统", meta = (DisplayName = "GetMostHatredCharacter"))
	class ACharacterBase* ReceiveGetMostHatredCharacter(const TArray<class ACharacterBase*>& Sources);

	virtual void TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, class  AActor* DamageCauser) { ReceiveTakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); }
	UFUNCTION(BlueprintImplementableEvent, Category = "仇恨系统", meta = (DisplayName = "TakeDamage"))
	class ACharacterBase* ReceiveTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);
};
