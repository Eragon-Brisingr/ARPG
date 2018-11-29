// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "XD_TimeSystemType.h"
#include "BTComposite_ARPG_TimeCircle.generated.h"

/**
 * 
 */
struct FTimeCircleMemory : public FBTCompositeMemory
{
	FXD_NativeSpecialGameTimeHandle Handle;
};

UCLASS(abstract)
class ARPG_API UBTComposite_ARPG_TimeCircleBase : public UBTCompositeNode
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_TimeCircleBase();

	void ExecuteNextTimeCircleEvent(FBehaviorTreeSearchData SearchData, int32 NextTimeCircleEventIndex) const;

	virtual void NotifyNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FTimeCircleMemory); }

#if WITH_EDITOR
	virtual bool CanAbortLowerPriority() const override;
	virtual FName GetNodeIconName() const override;
#endif
};

UCLASS(meta = (DisplayName = "每时循环"))
class ARPG_API UBTComposite_ARPG_HourTimeCircle : public UBTComposite_ARPG_TimeCircleBase
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_HourTimeCircle();

	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
    virtual FString GetStaticDescription() const override;

	FString GetRowBehaviorDesc(int32 StartIndex) const;

	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
    TArray<FXD_EveryHourConfig> HourCircleBehaviorConfig;
};

UCLASS(meta = (DisplayName = "每日循环"))
class ARPG_API UBTComposite_ARPG_DayTimeCircle : public UBTComposite_ARPG_TimeCircleBase
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_DayTimeCircle();

	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
	virtual FString GetStaticDescription() const override;

	FString GetRowBehaviorDesc(int32 StartIndex) const;

	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
    TArray<FXD_EveryDayConfig> DayCircleBehaviorConfig;
};

UCLASS(meta = (DisplayName = "每周循环"))
class ARPG_API UBTComposite_ARPG_WeekTimeCircle : public UBTComposite_ARPG_TimeCircleBase
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_WeekTimeCircle();

	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
    virtual FString GetStaticDescription() const override;

	FString GetRowBehaviorDesc(int32 StartIndex) const;

	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
    TArray<FXD_EveryWeekConfig> WeekCircleBehaviorConfig;
};

UCLASS(meta = (DisplayName = "每月循环"))
class ARPG_API UBTComposite_ARPG_MonthTimeCircle : public UBTComposite_ARPG_TimeCircleBase
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_MonthTimeCircle();

	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
	virtual FString GetStaticDescription() const override;

	FString GetRowBehaviorDesc(int32 StartIndex) const;
	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
	TArray<FXD_EveryMonthConfig> MonthCircleBehaviorConfig;
};

UCLASS(meta = (DisplayName = "每年循环"))
class ARPG_API UBTComposite_ARPG_YearTimeCircle : public UBTComposite_ARPG_TimeCircleBase
{
	GENERATED_BODY()
public:
	UBTComposite_ARPG_YearTimeCircle();

	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
	virtual FString GetStaticDescription() const override;

	FString GetRowBehaviorDesc(int32 StartIndex) const;
	void ResetConfigSize();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weighted Random", meta = (EditFixedOrder), EditFixedSize)
	TArray<FXD_EveryYearConfig> YearCircleBehaviorConfig;
};


