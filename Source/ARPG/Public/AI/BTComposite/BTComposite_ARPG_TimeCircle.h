// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "XD_TimeSystemType.h"
#include "BTComposite_ARPG_TimeCircle.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "每时循环"))
class ARPG_API UBTComposite_ARPG_HourTimeCircle : public UBTCompositeNode
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_HourTimeCircle();

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;
    virtual FString GetStaticDescription() const override;

	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    virtual bool CanAbortLowerPriority() const override;
    virtual FName GetNodeIconName() const override;
#endif
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
    TArray<FXD_EveryHourConfig> HourCircleBehaviorConfig;
};

UCLASS(meta = (DisplayName = "每日循环"))
class ARPG_API UBTComposite_ARPG_DayTimeCircle : public UBTCompositeNode
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_DayTimeCircle();

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;
    virtual FString GetStaticDescription() const override;

	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    virtual bool CanAbortLowerPriority() const override;
    virtual FName GetNodeIconName() const override;
#endif
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
    TArray<FXD_EveryDayConfig> DayCircleBehaviorConfig;
};

UCLASS(meta = (DisplayName = "每周循环"))
class ARPG_API UBTComposite_ARPG_WeekTimeCircle : public UBTCompositeNode
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_WeekTimeCircle();

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;
    virtual FString GetStaticDescription() const override;

	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    virtual bool CanAbortLowerPriority() const override;
    virtual FName GetNodeIconName() const override;
#endif
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
    TArray<FXD_EveryWeekConfig> WeekCircleBehaviorConfig;
};

UCLASS(meta = (DisplayName = "每月循环"))
class ARPG_API UBTComposite_ARPG_MonthTimeCircle : public UBTCompositeNode
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_MonthTimeCircle();

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;
	virtual FString GetStaticDescription() const override;

	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual bool CanAbortLowerPriority() const override;
	virtual FName GetNodeIconName() const override;
#endif
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
	TArray<FXD_EveryMonthConfig> MonthCircleBehaviorConfig;
};

UCLASS(meta = (DisplayName = "每年循环"))
class ARPG_API UBTComposite_ARPG_YearTimeCircle : public UBTCompositeNode
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_YearTimeCircle();

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;
	virtual FString GetStaticDescription() const override;

	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual bool CanAbortLowerPriority() const override;
	virtual FName GetNodeIconName() const override;
#endif
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
	TArray<FXD_EveryYearConfig> YearCircleBehaviorConfig;
};


