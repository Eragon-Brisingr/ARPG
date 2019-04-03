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

	virtual float GetSightAddAlertValue(const FVector& ObserverLocation, const FVector& SeenLocation, const class ACharacterBase* LookedCharacter) const { return ReceiveGetSightAddAlertValue(ObserverLocation, SeenLocation, LookedCharacter); }
	UFUNCTION(BlueprintImplementableEvent, Category = "警戒系统")
	float ReceiveGetSightAddAlertValue(const FVector& ObserverLocation, const FVector& SeenLocation, const class ACharacterBase* LookedCharacter) const;

	virtual float GetHearAddAlertValue(const class ACharacterBase* HeardCharacter, const FVector& SimulusLocation, float Strength) const { return ReceiveGetHearAddAlertValue(HeardCharacter, SimulusLocation, Strength); }
	UFUNCTION(BlueprintImplementableEvent, Category = "警戒系统")
	float ReceiveGetHearAddAlertValue(const class ACharacterBase* HeardCharacter, const FVector& SimulusLocation, float Strength) const;

	virtual float GetNoseAddAlertValue(const class ACharacterBase* NosedCharacter) const { return ReceiveGetNoseAddAlertValue(NosedCharacter); }
	UFUNCTION(BlueprintImplementableEvent, Category = "警戒系统")
	float ReceiveGetNoseAddAlertValue(const class ACharacterBase* NosedCharacter) const;

	UFUNCTION(BlueprintCallable, Category = "警戒系统")
	bool CanSee(const FVector& ObserverLocation, const FVector& SeenLocation, const class ACharacterBase* LookedCharacter) const;
	UFUNCTION(BlueprintCallable, Category = "警戒系统")
	bool CanHear(const class ACharacterBase* HeardCharacter, const FVector& SimulusLocation, float Strength) const { return GetHearAddAlertValue(HeardCharacter, SimulusLocation, Strength) != 0.f; }
	UFUNCTION(BlueprintCallable, Category = "警戒系统")
	bool CanNose(const class ACharacterBase* NosedCharacter) const { return GetNoseAddAlertValue(NosedCharacter) > 0.f; }

	UFUNCTION(BlueprintCallable, Category = "警戒系统")
	float GetSightAddAlertValueFilterRelationship(const FVector& ObserverLocation, const FVector& SeenLocation, const class ACharacterBase* LookedCharacter) const;

	UFUNCTION(BlueprintCallable, Category = "警戒系统")
	float GetHearAddAlertValueFilterRelationship(const class ACharacterBase* HeardCharacter, const FVector& SimulusLocation, float Strength) const;

	UFUNCTION(BlueprintCallable, Category = "警戒系统")
	float GetNoseAddAlertValueFilterRelationship(const class ACharacterBase* NosedCharacter) const;

};
