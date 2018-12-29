// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SubSystem/ARPG_AlertSystemBase.h"
#include "Curves/CurveFloat.h"
#include "ARPG_AlertSystemNormal.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_AlertSystemNormal : public UARPG_AlertSystemBase
{
	GENERATED_BODY()
public:
	UARPG_AlertSystemNormal();

	virtual float GetSightAddAlertValue(const FVector& ObserverLocation, const FVector& SeenLocation, const class ACharacterBase* LookedCharacter) const override;

	virtual float GetHearAddAlertValue(const class ACharacterBase* HeardCharacter, const FVector& SimulusLocation, float Strength) const override;

	virtual float GetNoseAddAlertValue(const class ACharacterBase* NosedCharacter) const override;

public:
	UPROPERTY(EditAnywhere, Category = "警戒系统", meta = (XAxisName = "距离", YAxisName = "增加值"))
	FRuntimeFloatCurve SightAlertCurve;

	UPROPERTY(EditAnywhere, Category = "警戒系统", meta = (XAxisName = "距离", YAxisName = "增加值"))
	FRuntimeFloatCurve HearAlertCurve;

	UPROPERTY(EditAnywhere, Category = "警戒系统", meta = (XAxisName = "距离", YAxisName = "增加值"))
	FRuntimeFloatCurve NoseAlertCurve;

	UPROPERTY(EditAnywhere, Category = "警戒系统")
	float AlertRate = 1.f;
};
