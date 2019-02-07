// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/GameTask/Elements/ARPG_EventFlowElementBase.h"
#include "XD_TimeSystemType.h"
#include "EFE_GameTime.generated.h"

/**
 * 
 */
UCLASS(abstract)
class ARPG_API UEFE_TimeRangeBase : public UARPG_EventFlowElementBase
{
	GENERATED_BODY()
		
protected:
	UFUNCTION()
	void WhenInTimeRange();
	UFUNCTION()
	void WhenOutTimeRange();

	FXD_GameTimeEvent GetInTimeRangeEvent() const;
	FXD_GameTimeEvent GetOutTimeRangeEvent() const;
};

UCLASS(meta = (DisplayName = "在每时", Category = "时间相关"))
class ARPG_API UEFE_InHourRange : public UEFE_TimeRangeBase
{
	GENERATED_BODY()
public:
	UEFE_InHourRange()
		:StartTime(0), EndTime(30)
	{}

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;
	void WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;

	FText ReceiveGetDescribe_Implementation() const override;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "起始时间"))
	FXD_EveryHourConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "终止时间"))
	FXD_EveryHourConfig EndTime;
};

UCLASS(meta = (DisplayName = "在每日", Category = "时间相关"))
class ARPG_API UEFE_InDayRange : public UEFE_TimeRangeBase
{
	GENERATED_BODY()
public:
	UEFE_InDayRange()
		:StartTime(6, 0), EndTime(18, 0)
	{}

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;
	void WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;

	FText ReceiveGetDescribe_Implementation() const override;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "起始时间"))
	FXD_EveryDayConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "终止时间"))
	FXD_EveryDayConfig EndTime;
};

UCLASS(meta = (DisplayName = "在每周", Category = "时间相关"))
class ARPG_API UEFE_InWeekRange : public UEFE_TimeRangeBase
{
	GENERATED_BODY()
public:
	UEFE_InWeekRange()
		:StartTime(EXD_DayOfWeek::Monday, 6, 0), EndTime(EXD_DayOfWeek::Wednesday, 18, 0)
	{}

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;
	void WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;

	FText ReceiveGetDescribe_Implementation() const override;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "起始时间"))
	FXD_EveryWeekConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "终止时间"))
	FXD_EveryWeekConfig EndTime;
};

UCLASS(meta = (DisplayName = "在每月", Category = "时间相关"))
class ARPG_API UEFE_InMonthRange : public UEFE_TimeRangeBase
{
	GENERATED_BODY()
public:
	UEFE_InMonthRange()
		:StartTime(0, 6, 0), EndTime(15, 18, 0)
	{}

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;
	void WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;

	FText ReceiveGetDescribe_Implementation() const override;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "起始时间"))
	FXD_EveryMonthConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "终止时间"))
	FXD_EveryMonthConfig EndTime;
};

UCLASS(meta = (DisplayName = "在每年", Category = "时间相关"))
class ARPG_API UEFE_InYearRange : public UEFE_TimeRangeBase
{
	GENERATED_BODY()
public:
	UEFE_InYearRange()
		:StartTime(1, 0, 6, 0), EndTime(7, 15, 18, 0)
	{}

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;
	void WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;

	FText ReceiveGetDescribe_Implementation() const override;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "起始时间"))
	FXD_EveryYearConfig StartTime;

	UPROPERTY(EditAnywhere, Category = "时间相关", SaveGame, Replicated, meta = (DisplayName = "终止时间"))
	FXD_EveryYearConfig EndTime;
};
