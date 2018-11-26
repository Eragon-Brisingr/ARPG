// Fill out your copyright notice in the Description page of Project Settings.

#include "BTComposite_ARPG_TimeCircle.h"
#include "ARPG_TimeManager.h"
#include "Engine/World.h"

template<typename T>
void SetNumDefaulted(TArray<T>& Array, int32 NewNum, bool bAllowShrinking = true)
{
	if (NewNum > Array.Num())
	{
		Array.AddDefaulted(NewNum - Array.Num());
	}
	else if (NewNum < Array.Num())
	{
		Array.RemoveAt(NewNum, Array.Num() - NewNum, bAllowShrinking);
	}
}

UBTComposite_ARPG_HourTimeCircle::UBTComposite_ARPG_HourTimeCircle()
{
	NodeName = TEXT("每时循环");
}

int32 UBTComposite_ARPG_HourTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (HourCircleBehaviorConfig.Num() > 0 && PrevChild == BTSpecialChild::NotInitialized)
	{
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(GWorld))
		{
			for (int i = 0; i < HourCircleBehaviorConfig.Num(); ++i)
			{
				if (TimeManager->CurrentTime < HourCircleBehaviorConfig[i])
				{
					return i != 0 ? i - 1 : HourCircleBehaviorConfig.Num() - 1;
				}
			}
			return HourCircleBehaviorConfig.Num() - 1;
		}
	}

	return BTSpecialChild::ReturnToParent;
}

FString UBTComposite_ARPG_HourTimeCircle::GetStaticDescription() const
{
	const_cast<UBTComposite_ARPG_HourTimeCircle*>(this)->ResetConfigSize();

	FString Desc;
	if (HourCircleBehaviorConfig.Num() > 1)
	{
		for (int i = 0; i < HourCircleBehaviorConfig.Num(); ++i)
		{
			if (UBTNode* Node = GetChildNode(i))
			{
				const TCHAR Format[] = TEXT("%s 至 %s : %s");
				Desc += FString(TEXT("\n"));
				if (i == HourCircleBehaviorConfig.Num() - 1)
				{
					Desc += FString::Printf(Format, *HourCircleBehaviorConfig[i].ToString(), *HourCircleBehaviorConfig[0].ToString(), *Node->GetNodeName());
				}
				else
				{
					Desc += FString::Printf(Format, *HourCircleBehaviorConfig[i].ToString(), *HourCircleBehaviorConfig[i + 1].ToString(), *Node->GetNodeName());
				}
			}
		}
	}

	return Desc;
}

void UBTComposite_ARPG_HourTimeCircle::ResetConfigSize()
{
	return SetNumDefaulted(HourCircleBehaviorConfig, GetChildrenNum());
}

#if WITH_EDITOR
void UBTComposite_ARPG_HourTimeCircle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBTComposite_ARPG_HourTimeCircle, HourCircleBehaviorConfig))
	{
		HourCircleBehaviorConfig.Sort();
	}
}

bool UBTComposite_ARPG_HourTimeCircle::CanAbortLowerPriority() const
{
	return false;
}

FName UBTComposite_ARPG_HourTimeCircle::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}
#endif // WITH_EDITOR


UBTComposite_ARPG_DayTimeCircle::UBTComposite_ARPG_DayTimeCircle()
{
	NodeName = TEXT("每日循环");
}

int32 UBTComposite_ARPG_DayTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (DayCircleBehaviorConfig.Num() > 0 && PrevChild == BTSpecialChild::NotInitialized)
	{
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(GWorld))
		{
			for (int i = 0; i < DayCircleBehaviorConfig.Num(); ++i)
			{
				if (TimeManager->CurrentTime < DayCircleBehaviorConfig[i])
				{
					return i != 0 ? i - 1 : DayCircleBehaviorConfig.Num() - 1;
				}
			}
			return DayCircleBehaviorConfig.Num() - 1;
		}
	}

	return BTSpecialChild::ReturnToParent;
}

FString UBTComposite_ARPG_DayTimeCircle::GetStaticDescription() const
{
	const_cast<UBTComposite_ARPG_DayTimeCircle*>(this)->ResetConfigSize();

	FString Desc;
	if (DayCircleBehaviorConfig.Num() > 1)
	{
		for (int i = 0; i < DayCircleBehaviorConfig.Num(); ++i)
		{
			if (UBTNode* Node = GetChildNode(i))
			{
				const TCHAR Format[] = TEXT("%s 至 %s : %s");
				Desc += FString(TEXT("\n"));
				if (i == DayCircleBehaviorConfig.Num() - 1)
				{
					Desc += FString::Printf(Format, *DayCircleBehaviorConfig[i].ToString(), *DayCircleBehaviorConfig[0].ToString(), *Node->GetNodeName());
				}
				else
				{
					Desc += FString::Printf(Format, *DayCircleBehaviorConfig[i].ToString(), *DayCircleBehaviorConfig[i + 1].ToString(), *Node->GetNodeName());
				}
			}
		}
	}

	return Desc;
}

void UBTComposite_ARPG_DayTimeCircle::ResetConfigSize()
{
	return SetNumDefaulted(DayCircleBehaviorConfig, GetChildrenNum());
}

#if WITH_EDITOR
void UBTComposite_ARPG_DayTimeCircle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBTComposite_ARPG_DayTimeCircle, DayCircleBehaviorConfig))
	{
		DayCircleBehaviorConfig.Sort();
	}
}

bool UBTComposite_ARPG_DayTimeCircle::CanAbortLowerPriority() const
{
	return false;
}

FName UBTComposite_ARPG_DayTimeCircle::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}
#endif // WITH_EDITOR


UBTComposite_ARPG_WeekTimeCircle::UBTComposite_ARPG_WeekTimeCircle()
{
	NodeName = TEXT("每周循环");
}

int32 UBTComposite_ARPG_WeekTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (WeekCircleBehaviorConfig.Num() > 0 && PrevChild == BTSpecialChild::NotInitialized)
	{
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(GWorld))
		{
			for (int i = 0; i < WeekCircleBehaviorConfig.Num(); ++i)
			{
				if (TimeManager->CurrentTime < WeekCircleBehaviorConfig[i])
				{
					return i != 0 ? i - 1 : WeekCircleBehaviorConfig.Num() - 1;
				}
			}
			return WeekCircleBehaviorConfig.Num() - 1;
		}
	}

	return BTSpecialChild::ReturnToParent;
}

FString UBTComposite_ARPG_WeekTimeCircle::GetStaticDescription() const
{
	const_cast<UBTComposite_ARPG_WeekTimeCircle*>(this)->ResetConfigSize();

	FString Desc;
	if (WeekCircleBehaviorConfig.Num() > 1)
	{
		for (int i = 0; i < WeekCircleBehaviorConfig.Num(); ++i)
		{
			if (UBTNode* Node = GetChildNode(i))
			{
				const TCHAR Format[] = TEXT("%s 至 %s : %s");
				Desc += FString(TEXT("\n"));
				if (i == WeekCircleBehaviorConfig.Num() - 1)
				{
					Desc += FString::Printf(Format, *WeekCircleBehaviorConfig[i].ToString(), *WeekCircleBehaviorConfig[0].ToString(), *Node->GetNodeName());
				}
				else
				{
					Desc += FString::Printf(Format, *WeekCircleBehaviorConfig[i].ToString(), *WeekCircleBehaviorConfig[i + 1].ToString(), *Node->GetNodeName());
				}
			}
		}
	}

	return Desc;
}

void UBTComposite_ARPG_WeekTimeCircle::ResetConfigSize()
{
	return SetNumDefaulted(WeekCircleBehaviorConfig, GetChildrenNum());
}

#if WITH_EDITOR
void UBTComposite_ARPG_WeekTimeCircle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBTComposite_ARPG_WeekTimeCircle, WeekCircleBehaviorConfig))
	{
		WeekCircleBehaviorConfig.Sort();
	}
}

bool UBTComposite_ARPG_WeekTimeCircle::CanAbortLowerPriority() const
{
	return false;
}

FName UBTComposite_ARPG_WeekTimeCircle::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}
#endif // WITH_EDITOR

UBTComposite_ARPG_MonthTimeCircle::UBTComposite_ARPG_MonthTimeCircle()
{
	NodeName = TEXT("每月循环");
}

int32 UBTComposite_ARPG_MonthTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (MonthCircleBehaviorConfig.Num() > 0 && PrevChild == BTSpecialChild::NotInitialized)
	{
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(GWorld))
		{
			for (int i = 0; i < MonthCircleBehaviorConfig.Num(); ++i)
			{
				if (TimeManager->CurrentTime < MonthCircleBehaviorConfig[i])
				{
					return i != 0 ? i - 1 : MonthCircleBehaviorConfig.Num() - 1;
				}
			}
			return MonthCircleBehaviorConfig.Num() - 1;
		}
	}

	return BTSpecialChild::ReturnToParent;
}

FString UBTComposite_ARPG_MonthTimeCircle::GetStaticDescription() const
{
	const_cast<UBTComposite_ARPG_MonthTimeCircle*>(this)->ResetConfigSize();

	FString Desc;
	if (MonthCircleBehaviorConfig.Num() > 1)
	{
		for (int i = 0; i < MonthCircleBehaviorConfig.Num(); ++i)
		{
			if (UBTNode* Node = GetChildNode(i))
			{
				const TCHAR Format[] = TEXT("%s 至 %s : %s");
				Desc += FString(TEXT("\n"));
				if (i == MonthCircleBehaviorConfig.Num() - 1)
				{
					Desc += FString::Printf(Format, *MonthCircleBehaviorConfig[i].ToString(), *MonthCircleBehaviorConfig[0].ToString(), *Node->GetNodeName());
				}
				else
				{
					Desc += FString::Printf(Format, *MonthCircleBehaviorConfig[i].ToString(), *MonthCircleBehaviorConfig[i + 1].ToString(), *Node->GetNodeName());
				}
			}
		}
	}

	return Desc;
}

void UBTComposite_ARPG_MonthTimeCircle::ResetConfigSize()
{
	return SetNumDefaulted(MonthCircleBehaviorConfig, GetChildrenNum());
}

#if WITH_EDITOR
void UBTComposite_ARPG_MonthTimeCircle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBTComposite_ARPG_MonthTimeCircle, MonthCircleBehaviorConfig))
	{
		MonthCircleBehaviorConfig.Sort();
	}
}

bool UBTComposite_ARPG_MonthTimeCircle::CanAbortLowerPriority() const
{
	return false;
}

FName UBTComposite_ARPG_MonthTimeCircle::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}
#endif // WITH_EDITOR


UBTComposite_ARPG_YearTimeCircle::UBTComposite_ARPG_YearTimeCircle()
{
	NodeName = TEXT("每年循环");
}

int32 UBTComposite_ARPG_YearTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (YearCircleBehaviorConfig.Num() > 0 && PrevChild == BTSpecialChild::NotInitialized)
	{
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(GWorld))
		{
			for (int i = 0; i < YearCircleBehaviorConfig.Num(); ++i)
			{
				if (TimeManager->CurrentTime < YearCircleBehaviorConfig[i])
				{
					return i != 0 ? i - 1 : YearCircleBehaviorConfig.Num() - 1;
				}
			}
			return YearCircleBehaviorConfig.Num() - 1;
		}
	}

	return BTSpecialChild::ReturnToParent;
}

FString UBTComposite_ARPG_YearTimeCircle::GetStaticDescription() const
{
	const_cast<UBTComposite_ARPG_YearTimeCircle*>(this)->ResetConfigSize();

	FString Desc;
	if (YearCircleBehaviorConfig.Num() > 1)
	{
		for (int i = 0; i < YearCircleBehaviorConfig.Num(); ++i)
		{
			if (UBTNode* Node = GetChildNode(i))
			{
				const TCHAR Format[] = TEXT("%s 至 %s : %s");
				Desc += FString(TEXT("\n"));
				if (i == YearCircleBehaviorConfig.Num() - 1)
				{
					Desc += FString::Printf(Format, *YearCircleBehaviorConfig[i].ToString(), *YearCircleBehaviorConfig[0].ToString(), *Node->GetNodeName());
				}
				else
				{
					Desc += FString::Printf(Format, *YearCircleBehaviorConfig[i].ToString(), *YearCircleBehaviorConfig[i + 1].ToString(), *Node->GetNodeName());
				}
			}
		}
	}

	return Desc;
}

void UBTComposite_ARPG_YearTimeCircle::ResetConfigSize()
{
	return SetNumDefaulted(YearCircleBehaviorConfig, GetChildrenNum());
}

#if WITH_EDITOR
void UBTComposite_ARPG_YearTimeCircle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBTComposite_ARPG_YearTimeCircle, YearCircleBehaviorConfig))
	{
		YearCircleBehaviorConfig.Sort();
	}
}

bool UBTComposite_ARPG_YearTimeCircle::CanAbortLowerPriority() const
{
	return false;
}

FName UBTComposite_ARPG_YearTimeCircle::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}
#endif // WITH_EDITOR
