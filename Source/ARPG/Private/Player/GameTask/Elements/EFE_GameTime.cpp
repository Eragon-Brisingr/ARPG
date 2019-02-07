// Fill out your copyright notice in the Description page of Project Settings.

#include "EFE_GameTime.h"
#include <UnrealNetwork.h>
#include "ARPG_TimeManager.h"

#define LOCTEXT_NAMESPACE "ARPG_EventFlow"

void UEFE_TimeRangeBase::WhenInTimeRange()
{
	FinishEventFlowElement();
}

void UEFE_TimeRangeBase::WhenOutTimeRange()
{
	SetUnfinished();
}

FXD_GameTimeEvent UEFE_TimeRangeBase::GetInTimeRangeEvent() const
{
	FXD_GameTimeEvent InTimeRnageEvent;
	InTimeRnageEvent.BindDynamic(this, &UEFE_InDayRange::WhenInTimeRange);
	return InTimeRnageEvent;
}

FXD_GameTimeEvent UEFE_TimeRangeBase::GetOutTimeRangeEvent() const
{
	FXD_GameTimeEvent OutTimeRnageEvent;
	OutTimeRnageEvent.BindDynamic(this, &UEFE_InDayRange::WhenOutTimeRange);
	return OutTimeRnageEvent;
}

void UEFE_InHourRange::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEFE_InHourRange, StartTime);
	DOREPLIFETIME(UEFE_InHourRange, EndTime);
}

void UEFE_InHourRange::WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->AddEveryHourEvent_Duration(StartTime, EndTime, GetInTimeRangeEvent());
		GameTimeManager->AddEveryHourEvent_Duration(EndTime, StartTime, GetOutTimeRangeEvent());
	}
}

void UEFE_InHourRange::WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->RemoveEveryHourEvent(StartTime, GetInTimeRangeEvent());
		GameTimeManager->RemoveEveryHourEvent(EndTime, GetOutTimeRangeEvent());
	}
}

FText UEFE_InHourRange::ReceiveGetDescribe_Implementation() const
{
	if (StartTime > EndTime)
	{
		return FText::Format(LOCTEXT("{0} 至 {1}", "{0} 至 {1}"), StartTime.ToText(), EndTime.ToText());
	}
	else
	{
		return FText::Format(LOCTEXT("{0} 至 次日{ 1 }", "{0} 至 次时 {1}"), EndTime.ToText(), StartTime.ToText());
	}
}

void UEFE_InDayRange::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEFE_InDayRange, StartTime);
	DOREPLIFETIME(UEFE_InDayRange, EndTime);
}

void UEFE_InDayRange::WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->AddEveryDayEvent_Duration(StartTime, EndTime, GetInTimeRangeEvent());
		GameTimeManager->AddEveryDayEvent_Duration(EndTime, StartTime, GetOutTimeRangeEvent());
	}
}

void UEFE_InDayRange::WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->RemoveEveryDayEvent(StartTime, GetInTimeRangeEvent());
		GameTimeManager->RemoveEveryDayEvent(EndTime, GetOutTimeRangeEvent());
	}
}

FText UEFE_InDayRange::ReceiveGetDescribe_Implementation() const
{
	if (StartTime > EndTime)
	{
		return FText::Format(LOCTEXT("{0} 至 {1}", "{0} 至 {1}"), StartTime.ToText(), EndTime.ToText());
	}
	else
	{
		return FText::Format(LOCTEXT("{0} 至 次日{ 1 }", "{0} 至 次日 {1}"), EndTime.ToText(), StartTime.ToText());
	}
}

void UEFE_InWeekRange::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEFE_InWeekRange, StartTime);
	DOREPLIFETIME(UEFE_InWeekRange, EndTime);
}

void UEFE_InWeekRange::WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->AddEveryWeekEvent_Duration(StartTime, EndTime, GetInTimeRangeEvent());
		GameTimeManager->AddEveryWeekEvent_Duration(EndTime, StartTime, GetOutTimeRangeEvent());
	}
}

void UEFE_InWeekRange::WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->RemoveEveryWeekEvent(StartTime, GetInTimeRangeEvent());
		GameTimeManager->RemoveEveryWeekEvent(EndTime, GetOutTimeRangeEvent());
	}
}

FText UEFE_InWeekRange::ReceiveGetDescribe_Implementation() const
{
	if (StartTime > EndTime)
	{
		return FText::Format(LOCTEXT("{0} 至 {1}", "{0} 至 {1}"), StartTime.ToText(), EndTime.ToText());
	}
	else
	{
		return FText::Format(LOCTEXT("{0} 至 次日{ 1 }", "{0} 至 次周 {1}"), EndTime.ToText(), StartTime.ToText());
	}
}

void UEFE_InMonthRange::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEFE_InMonthRange, StartTime);
	DOREPLIFETIME(UEFE_InMonthRange, EndTime);
}

void UEFE_InMonthRange::WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->AddEveryMonthEvent_Duration(StartTime, EndTime, GetInTimeRangeEvent());
		GameTimeManager->AddEveryMonthEvent_Duration(EndTime, StartTime, GetOutTimeRangeEvent());
	}
}

void UEFE_InMonthRange::WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->RemoveEveryMonthEvent(StartTime, GetInTimeRangeEvent());
		GameTimeManager->RemoveEveryMonthEvent(EndTime, GetOutTimeRangeEvent());
	}
}

FText UEFE_InMonthRange::ReceiveGetDescribe_Implementation() const
{
	if (StartTime > EndTime)
	{
		return FText::Format(LOCTEXT("{0} 至 {1}", "{0} 至 {1}"), StartTime.ToText(), EndTime.ToText());
	}
	else
	{
		return FText::Format(LOCTEXT("{0} 至 次日{ 1 }", "{0} 至 次月 {1}"), EndTime.ToText(), StartTime.ToText());
	}
}

void UEFE_InYearRange::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEFE_InYearRange, StartTime);
	DOREPLIFETIME(UEFE_InYearRange, EndTime);
}

void UEFE_InYearRange::WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->AddEveryYearEvent_Duration(StartTime, EndTime, GetInTimeRangeEvent());
		GameTimeManager->AddEveryYearEvent_Duration(EndTime, StartTime, GetOutTimeRangeEvent());
	}
}

void UEFE_InYearRange::WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	if (UXD_TimeManager* GameTimeManager = UARPG_TimeManager::GetGameTimeManager(EventFlowOwner))
	{
		GameTimeManager->RemoveEveryYearEvent(StartTime, GetInTimeRangeEvent());
		GameTimeManager->RemoveEveryYearEvent(EndTime, GetOutTimeRangeEvent());
	}
}

FText UEFE_InYearRange::ReceiveGetDescribe_Implementation() const
{
	if (StartTime > EndTime)
	{
		return FText::Format(LOCTEXT("{0} 至 {1}", "{0} 至 {1}"), StartTime.ToText(), EndTime.ToText());
	}
	else
	{
		return FText::Format(LOCTEXT("{0} 至 次日{ 1 }", "{0} 至 次年 {1}"), EndTime.ToText(), StartTime.ToText());
	}
}

#undef LOCTEXT_NAMESPACE
