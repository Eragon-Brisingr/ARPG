// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "XD_TimeSystemType.h"
#include "BTDecorator_ARPG_Time.generated.h"

/**
 * 
 */
UCLASS(abstract)
class ARPG_API UBTDecorator_ARPG_TimeBase : public UBTDecorator
{
	GENERATED_BODY()
public:

private:
	virtual FString GetStaticDescription() const override;
};

UCLASS()
class ARPG_API UBTDecorator_ARPG_Time_InHourRange : public UBTDecorator_ARPG_TimeBase
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryHourConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryHourConfig EndTime;
};

UCLASS()
class ARPG_API UBTDecorator_ARPG_Time_InDayRange : public UBTDecorator_ARPG_TimeBase
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryDayConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryDayConfig EndTime;
};

UCLASS()
class ARPG_API UBTDecorator_ARPG_Time_InWeekRange : public UBTDecorator_ARPG_TimeBase
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryWeekConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryWeekConfig EndTime;
};

UCLASS()
class ARPG_API UBTDecorator_ARPG_Time_InMonthRange : public UBTDecorator_ARPG_TimeBase
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryMonthConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryMonthConfig EndTime;
};

UCLASS()
class ARPG_API UBTDecorator_ARPG_Time_InYearRange : public UBTDecorator_ARPG_TimeBase
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryYearConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_EveryYearConfig EndTime;
};

UCLASS()
class ARPG_API UBTDecorator_ARPG_Time_InSpecialRange : public UBTDecorator_ARPG_TimeBase
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_SpecialTimeConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间")
	FXD_SpecialTimeConfig EndTime;
};


