// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_SneakSystemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, collapsecategories, abstract)
class ARPG_API UARPG_SneakSystemBase : public UObject
{
	GENERATED_BODY()
public:
	virtual class UWorld* GetWorld() const override;

	UPROPERTY(BlueprintReadOnly, Category = "潜行系统")
	class ACharacterBase* Character;

	void InitSneakSystem(class ACharacterBase* Owner)
	{
		Character = Owner;
		WhenInitSneakSystem();
	}
	virtual void WhenInitSneakSystem() { ReceiveWhenSneakSystem(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "潜行系统", meta = (DisplayName = "WhenInitSneakSystem"))
	void ReceiveWhenSneakSystem();
	
public:
	UPROPERTY(EditAnywhere, Category = "潜行系统")
	float SightNormalValue = 1.f;

	UPROPERTY(EditAnywhere, Category = "潜行系统")
	float HearNormalValue = 1.f;

	UPROPERTY(EditAnywhere, Category = "潜行系统")
	float NoseNormalValue = 1.f;

	UFUNCTION(BlueprintCallable, Category = "潜行系统")
	virtual float GetSightNormalValue() const { return SightNormalValue * ReceiveGetSightNormalValue(); }
	UFUNCTION(BlueprintNativeEvent, Category = "潜行系统", meta = (DisplayName = "GetSightAlertValue"))
	float ReceiveGetSightNormalValue() const;
	float ReceiveGetSightNormalValue_Implementation() const { return 1.f; }

	UFUNCTION(BlueprintCallable, Category = "潜行系统")
	virtual float GetHearNormalValue() const { return HearNormalValue * ReceiveGetHearNormalValue(); }
	UFUNCTION(BlueprintNativeEvent, Category = "潜行系统", meta = (DisplayName = "GetHearNormalValue"))
	float ReceiveGetHearNormalValue() const;
	float ReceiveGetHearNormalValue_Implementation() const { return 1.f; }

	UFUNCTION(BlueprintCallable, Category = "潜行系统")
	virtual float GetNoseNormalValue() const { return NoseNormalValue * ReceiveGetNoseNormalValue(); }
	UFUNCTION(BlueprintNativeEvent, Category = "潜行系统", meta = (DisplayName = "GetNoseNormalValue"))
	float ReceiveGetNoseNormalValue() const;
	float ReceiveGetNoseNormalValue_Implementation() const { return 1.f; }
public:
	UPROPERTY(EditAnywhere, Category = "潜行系统")
	float SightSneakValue = 5.f;

	UPROPERTY(EditAnywhere, Category = "潜行系统")
	float HearSneakValue = 5.f;

	UPROPERTY(EditAnywhere, Category = "潜行系统")
	float NoseSneakValue = 5.f;

	UFUNCTION(BlueprintCallable, Category = "潜行系统")
	virtual float GetSightSneakValue() const { return SightSneakValue * ReceiveGetSightSneakValue(); }
	UFUNCTION(BlueprintNativeEvent, Category = "潜行系统", meta = (DisplayName = "GetSightAlertValue"))
	float ReceiveGetSightSneakValue() const;
	float ReceiveGetSightSneakValue_Implementation() const { return 1.f; }

	UFUNCTION(BlueprintCallable, Category = "潜行系统")
	virtual float GetHearSneakValue() const { return HearSneakValue * ReceiveGetHearSneakValue(); }
	UFUNCTION(BlueprintNativeEvent, Category = "潜行系统", meta = (DisplayName = "GetHearSneakValue"))
	float ReceiveGetHearSneakValue() const;
	float ReceiveGetHearSneakValue_Implementation() const { return 1.f; }

	UFUNCTION(BlueprintCallable, Category = "潜行系统")
	virtual float GetNoseSneakValue() const { return NoseSneakValue * ReceiveGetNoseSneakValue(); }
	UFUNCTION(BlueprintNativeEvent, Category = "潜行系统", meta = (DisplayName = "GetNoseSneakValue"))
	float ReceiveGetNoseSneakValue() const;
	float ReceiveGetNoseSneakValue_Implementation() const { return 1.f; }

public:
	UFUNCTION(BlueprintCallable, Category = "潜行系统")
	float GetSightHideValue() const;

	UFUNCTION(BlueprintCallable, Category = "潜行系统")
	float GetHearHideValue() const;

	UFUNCTION(BlueprintCallable, Category = "潜行系统")
	float GetNoseHideValue() const;
public:
	bool IsSneaking() const;
};
