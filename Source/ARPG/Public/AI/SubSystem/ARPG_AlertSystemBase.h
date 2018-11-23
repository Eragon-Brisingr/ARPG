// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_AlertSystemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, collapsecategories, abstract)
class ARPG_API UARPG_AlertSystemBase : public UObject
{
	GENERATED_BODY()
public:
	virtual class UWorld* GetWorld() const override;

	UPROPERTY(BlueprintReadOnly, Category = "警戒系统")
	class ACharacterBase* Character;

	void InitAlertSystem(class ACharacterBase* Owner)
	{
		Character = Owner;
		WhenInitAlertSystem();
	}
	virtual void WhenInitAlertSystem() { ReceiveWhenAlertSystem(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "警戒系统", meta = (DisplayName = "WhenInitAlertSystem"))
	void ReceiveWhenAlertSystem();

	UFUNCTION(BlueprintCallable, Category = "警戒系统")
	virtual float GetSightAddAlertValue(const class ACharacterBase* LookedCharacter) const { return ReceiveGetSightAddAlertValue(LookedCharacter); }
	UFUNCTION(BlueprintImplementableEvent, Category = "警戒系统")
	float ReceiveGetSightAddAlertValue(const class ACharacterBase* LookedCharacter) const;

	UFUNCTION(BlueprintCallable, Category = "警戒系统")
	virtual float GetHearAddAlertValue(const class ACharacterBase* HeardCharacter, const FVector& SimulusLocation, float Strength) const { return ReceiveGetHearAddAlertValue(HeardCharacter, SimulusLocation, Strength); }
	UFUNCTION(BlueprintImplementableEvent, Category = "警戒系统")
	float ReceiveGetHearAddAlertValue(const class ACharacterBase* HeardCharacter, const FVector& SimulusLocation, float Strength) const;

	UFUNCTION(BlueprintCallable, Category = "警戒系统")
	virtual float GetNoseAddAlertValue(const class ACharacterBase* NosedCharacter) const { return ReceiveGetNoseAddAlertValue(NosedCharacter); }
	UFUNCTION(BlueprintImplementableEvent, Category = "警戒系统")
	float ReceiveGetNoseAddAlertValue(const class ACharacterBase* NosedCharacter) const;
};
