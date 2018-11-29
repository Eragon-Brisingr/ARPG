// Fill out your copyright notice in the Description page of Project Settings.

#include "BTComposite_ARPG_TimeCircle.h"
#include "ARPG_TimeManager.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "ARPG_AI_Log.h"

template<typename T>
void SetNumDefaulted(TArray<T>& Array, int32 NewNum, bool bAllowShrinking = true)
{
	if (NewNum > Array.Num())
	{
		int32 AddNumber = NewNum - Array.Num();
		for (int32 i = 0; i < AddNumber; ++i)
		{
			Array.Add(T());
		}
	}
	else if (NewNum < Array.Num())
	{
		Array.RemoveAt(NewNum, Array.Num() - NewNum, bAllowShrinking);
	}
}

class UBTC_Ex : public UBehaviorTreeComponent
{
public:
	static void SetExecutionRequestSearchEnd(UBehaviorTreeComponent& BTC)
	{
		static_cast<UBTC_Ex&>(BTC).ExecutionRequest.SearchEnd.ExecutionIndex = 0xffff;
	}

	static UBTCompositeNode* GetRootNode(UBehaviorTreeComponent& BTC)
	{
		UBTC_Ex& BTCE = static_cast<UBTC_Ex&>(BTC);
		return BTCE.InstanceStack[BTCE.ActiveInstanceIdx].RootNode;
	}
};

UBTComposite_ARPG_TimeCircleBase::UBTComposite_ARPG_TimeCircleBase()
{
	bUseNodeDeactivationNotify = true;
}

void UBTComposite_ARPG_TimeCircleBase::ExecuteNextTimeCircleEvent(FBehaviorTreeSearchData SearchData, int32 NextTimeCircleEventIndex) const
{
	FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);

	const int32 InstanceIdx = SearchData.OwnerComp.FindInstanceContainingNode(GetParentNode());

	SearchData.OwnerComp.RequestExecution(GetParentNode() ? GetParentNode() : UBTC_Ex::GetRootNode(SearchData.OwnerComp), InstanceIdx, this, NextTimeCircleEventIndex, EBTNodeResult::Succeeded);
}

void UBTComposite_ARPG_TimeCircleBase::NotifyNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const
{
	if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
	{
		FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);

		TimeManager->RemoveNativeSpecialGameTimeEvent(TimeCircleMemory->Handle);
		AI_V_Display_LOG(SearchData.OwnerComp, "[%s]时间行为循环被中断", *GetNodeName());
	}
}

#if WITH_EDITOR
bool UBTComposite_ARPG_TimeCircleBase::CanAbortLowerPriority() const
{
	return true;
}

FName UBTComposite_ARPG_TimeCircleBase::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}

#endif // WITH_EDITOR

UBTComposite_ARPG_HourTimeCircle::UBTComposite_ARPG_HourTimeCircle()
{
	NodeName = TEXT("每时循环");
}

int32 UBTComposite_ARPG_HourTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (HourCircleBehaviorConfig.Num() > 0)
	{
		FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);

		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
		{
			const int32 ConfigNum = HourCircleBehaviorConfig.Num();
			const FXD_GameTime& CurrentTime = TimeManager->CurrentTime;

			int32 NextBehaviorIndex = PrevChild != BTSpecialChild::NotInitialized ? (PrevChild + (LastResult == EBTNodeResult::Failed ? 2 : 1)) % ConfigNum : 0;
			for (; NextBehaviorIndex < ConfigNum; ++NextBehaviorIndex)
			{
				if (CurrentTime < HourCircleBehaviorConfig[NextBehaviorIndex])
				{
					break;
				}
			}

			bool bIsNextCircle = NextBehaviorIndex == ConfigNum;
			NextBehaviorIndex %= ConfigNum;

			int32 BehaviorIndex = ((NextBehaviorIndex - 1) + ConfigNum) % ConfigNum;
			if (BehaviorIndex != PrevChild)
			{
				int32 Minute;
				HourCircleBehaviorConfig[NextBehaviorIndex].GetConfig(Minute);
				FXD_SpecialTimeConfig NextExecuteTimeBehaviorStartTime = FXD_GameTimeSpan(CurrentTime.GetTotalDays(), CurrentTime.GetHour(), Minute);
				if (bIsNextCircle)
				{
					NextExecuteTimeBehaviorStartTime = FXD_GameTime(NextExecuteTimeBehaviorStartTime.GetTicks() + FXD_GameTimeSpan(0, 1, 0).GetTicks());
				}
				TimeCircleMemory->Handle = TimeManager->AddNativeSpecialGameTimeEvent(NextExecuteTimeBehaviorStartTime, UXD_TimeManager::FXD_GameTimeNativeDelegate::CreateUObject(this, &UBTComposite_ARPG_HourTimeCircle::ExecuteNextTimeCircleEvent, SearchData, NextBehaviorIndex));

				AI_V_Display_LOG(SearchData.OwnerComp, "[%s]", *GetRowBehaviorDesc(BehaviorIndex));

				return BehaviorIndex;
			}
		}
	}

	return BTSpecialChild::ReturnToParent;
}

FString UBTComposite_ARPG_HourTimeCircle::GetRowBehaviorDesc(int32 StartIndex) const
{
	if (StartIndex == HourCircleBehaviorConfig.Num() - 1)
	{
		return FString::Printf(TEXT("%s 至下一个 %s : %s"), *HourCircleBehaviorConfig[StartIndex].ToString(), *HourCircleBehaviorConfig[0].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
	else
	{
		return FString::Printf(TEXT("%s 至 %s : %s"), *HourCircleBehaviorConfig[StartIndex].ToString(), *HourCircleBehaviorConfig[StartIndex + 1].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
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
				Desc += FString(TEXT("\n")) + GetRowBehaviorDesc(i);
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
#endif // WITH_EDITOR


UBTComposite_ARPG_DayTimeCircle::UBTComposite_ARPG_DayTimeCircle()
{
	NodeName = TEXT("每日循环");
}

int32 UBTComposite_ARPG_DayTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (DayCircleBehaviorConfig.Num() > 0)
	{
		FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);

		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
		{
			const int32 ConfigNum = DayCircleBehaviorConfig.Num();
			const FXD_GameTime& CurrentTime = TimeManager->CurrentTime;

			int32 NextBehaviorIndex = PrevChild != BTSpecialChild::NotInitialized ? (PrevChild + (LastResult == EBTNodeResult::Failed ? 2 : 1)) % ConfigNum : 0;
			for (; NextBehaviorIndex < ConfigNum; ++NextBehaviorIndex)
			{
				if (CurrentTime < DayCircleBehaviorConfig[NextBehaviorIndex])
				{
					break;
				}
			}

			bool bIsNextCircle = NextBehaviorIndex == ConfigNum;
			NextBehaviorIndex %= ConfigNum;

			int32 BehaviorIndex = ((NextBehaviorIndex - 1) + ConfigNum) % ConfigNum;
			if (BehaviorIndex != PrevChild)
			{
				int32 Hour, Minute;
				DayCircleBehaviorConfig[NextBehaviorIndex].GetConfig(Hour, Minute);
				FXD_SpecialTimeConfig NextExecuteTimeBehaviorStartTime = FXD_GameTimeSpan(CurrentTime.GetTotalDays(), Hour, Minute);
				if (bIsNextCircle)
				{
					NextExecuteTimeBehaviorStartTime = FXD_GameTime(NextExecuteTimeBehaviorStartTime.GetTicks() + FXD_GameTimeSpan(1, 0, 0).GetTicks());
				}
				TimeCircleMemory->Handle = TimeManager->AddNativeSpecialGameTimeEvent(NextExecuteTimeBehaviorStartTime, UXD_TimeManager::FXD_GameTimeNativeDelegate::CreateUObject(this, &UBTComposite_ARPG_HourTimeCircle::ExecuteNextTimeCircleEvent, SearchData, NextBehaviorIndex));

				AI_V_Display_LOG(SearchData.OwnerComp, "[%s][%s]", *CurrentTime.ToString(), *GetRowBehaviorDesc(BehaviorIndex));

				return BehaviorIndex;
			}
		}
	}

	return BTSpecialChild::ReturnToParent;
}

FString UBTComposite_ARPG_DayTimeCircle::GetRowBehaviorDesc(int32 StartIndex) const
{
	if (StartIndex == DayCircleBehaviorConfig.Num() - 1)
	{
		return FString::Printf(TEXT("%s 至下一个 %s : %s"), *DayCircleBehaviorConfig[StartIndex].ToString(), *DayCircleBehaviorConfig[0].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
	else
	{
		return FString::Printf(TEXT("%s 至 %s : %s"), *DayCircleBehaviorConfig[StartIndex].ToString(), *DayCircleBehaviorConfig[StartIndex + 1].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
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
				Desc += FString(TEXT("\n")) + GetRowBehaviorDesc(i);
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
#endif // WITH_EDITOR


UBTComposite_ARPG_WeekTimeCircle::UBTComposite_ARPG_WeekTimeCircle()
{
	NodeName = TEXT("每周循环");
}

int32 UBTComposite_ARPG_WeekTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (WeekCircleBehaviorConfig.Num() > 0)
	{
		FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);

		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
		{
			const int32 ConfigNum = WeekCircleBehaviorConfig.Num();
			const FXD_GameTime& CurrentTime = TimeManager->CurrentTime;

			int32 NextBehaviorIndex = PrevChild != BTSpecialChild::NotInitialized ? (PrevChild + (LastResult == EBTNodeResult::Failed ? 2 : 1)) % ConfigNum : 0;
			for (; NextBehaviorIndex < ConfigNum; ++NextBehaviorIndex)
			{
				if (CurrentTime < WeekCircleBehaviorConfig[NextBehaviorIndex])
				{
					break;
				}
			}

			bool bIsNextCircle = NextBehaviorIndex == ConfigNum;
			NextBehaviorIndex %= ConfigNum;

			int32 BehaviorIndex = ((NextBehaviorIndex - 1) + ConfigNum) % ConfigNum;
			if (BehaviorIndex != PrevChild)
			{
				int32 WeekDay, Hour, Minute;
				WeekCircleBehaviorConfig[NextBehaviorIndex].GetConfig(WeekDay, Hour, Minute);
				FXD_SpecialTimeConfig NextExecuteTimeBehaviorStartTime = FXD_GameTimeSpan(CurrentTime.GetTotalDays() + WeekDay - CurrentTime.GetDayOfWeekNum(), Hour, Minute);
				if (bIsNextCircle)
				{
					NextExecuteTimeBehaviorStartTime = FXD_GameTime(NextExecuteTimeBehaviorStartTime.GetTicks() + FXD_GameTimeSpan(7, 0, 0).GetTicks());
				}
				TimeCircleMemory->Handle = TimeManager->AddNativeSpecialGameTimeEvent(NextExecuteTimeBehaviorStartTime, UXD_TimeManager::FXD_GameTimeNativeDelegate::CreateUObject(this, &UBTComposite_ARPG_HourTimeCircle::ExecuteNextTimeCircleEvent, SearchData, NextBehaviorIndex));

				AI_V_Display_LOG(SearchData.OwnerComp, "[%s]", *GetRowBehaviorDesc(BehaviorIndex));

				return BehaviorIndex;
			}
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
				Desc += FString(TEXT("\n")) + GetRowBehaviorDesc(i);
			}
		}
	}

	return Desc;
}

FString UBTComposite_ARPG_WeekTimeCircle::GetRowBehaviorDesc(int32 StartIndex) const
{
	if (StartIndex == WeekCircleBehaviorConfig.Num() - 1)
	{
		return FString::Printf(TEXT("%s 至下一个 %s : %s"), *WeekCircleBehaviorConfig[StartIndex].ToString(), *WeekCircleBehaviorConfig[0].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
	else
	{
		return FString::Printf(TEXT("%s 至 %s : %s"), *WeekCircleBehaviorConfig[StartIndex].ToString(), *WeekCircleBehaviorConfig[StartIndex + 1].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
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
#endif // WITH_EDITOR

UBTComposite_ARPG_MonthTimeCircle::UBTComposite_ARPG_MonthTimeCircle()
{
	NodeName = TEXT("每月循环");
}

int32 UBTComposite_ARPG_MonthTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (MonthCircleBehaviorConfig.Num() > 0)
	{
		FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);

		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
		{
			const int32 ConfigNum = MonthCircleBehaviorConfig.Num();
			const FXD_GameTime& CurrentTime = TimeManager->CurrentTime;

			int32 NextBehaviorIndex = PrevChild != BTSpecialChild::NotInitialized ? (PrevChild + (LastResult == EBTNodeResult::Failed ? 2 : 1)) % ConfigNum : 0;
			for (; NextBehaviorIndex < ConfigNum; ++NextBehaviorIndex)
			{
				if (CurrentTime < MonthCircleBehaviorConfig[NextBehaviorIndex])
				{
					break;
				}
			}

			bool bIsNextCircle = NextBehaviorIndex == ConfigNum;
			NextBehaviorIndex %= ConfigNum;

			int32 BehaviorIndex = ((NextBehaviorIndex - 1) + ConfigNum) % ConfigNum;
			if (BehaviorIndex != PrevChild)
			{
				int32 Year, Month, Day;
				CurrentTime.GetDate(Year, Month, Day);
				if (bIsNextCircle)
				{
					Month += 1;
					if (Month >= 12)
					{
						Month = 1;
						Year += 1;
					}
				}

				int32 DayOfMonth, Hour, Minute;
				MonthCircleBehaviorConfig[NextBehaviorIndex].GetConfigSafe(Year, Month, DayOfMonth, Hour, Minute);

				FXD_SpecialTimeConfig NextExecuteTimeBehaviorStartTime = FXD_SpecialTimeConfig(Year, Month, DayOfMonth, Hour, Minute);
				TimeCircleMemory->Handle = TimeManager->AddNativeSpecialGameTimeEvent(NextExecuteTimeBehaviorStartTime, UXD_TimeManager::FXD_GameTimeNativeDelegate::CreateUObject(this, &UBTComposite_ARPG_HourTimeCircle::ExecuteNextTimeCircleEvent, SearchData, NextBehaviorIndex));

				AI_V_Display_LOG(SearchData.OwnerComp, "[%s][%s]", *CurrentTime.ToString(), *GetRowBehaviorDesc(BehaviorIndex));

				return BehaviorIndex;
			}
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
				Desc += FString(TEXT("\n")) + GetRowBehaviorDesc(i);
			}
		}
	}

	return Desc;
}

FString UBTComposite_ARPG_MonthTimeCircle::GetRowBehaviorDesc(int32 StartIndex) const
{
	if (StartIndex == MonthCircleBehaviorConfig.Num() - 1)
	{
		return FString::Printf(TEXT("%s 至下一个 %s : %s"), *MonthCircleBehaviorConfig[StartIndex].ToString(), *MonthCircleBehaviorConfig[0].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
	else
	{
		return FString::Printf(TEXT("%s 至 %s : %s"), *MonthCircleBehaviorConfig[StartIndex].ToString(), *MonthCircleBehaviorConfig[StartIndex + 1].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
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
#endif // WITH_EDITOR


UBTComposite_ARPG_YearTimeCircle::UBTComposite_ARPG_YearTimeCircle()
{
	NodeName = TEXT("每年循环");
}

int32 UBTComposite_ARPG_YearTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (YearCircleBehaviorConfig.Num() > 0)
	{
		FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);

		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
		{
			const int32 ConfigNum = YearCircleBehaviorConfig.Num();
			const FXD_GameTime& CurrentTime = TimeManager->CurrentTime;

			int32 NextBehaviorIndex = PrevChild != BTSpecialChild::NotInitialized ? (PrevChild + (LastResult == EBTNodeResult::Failed ? 2 : 1)) % ConfigNum : 0;
			for (; NextBehaviorIndex < ConfigNum; ++NextBehaviorIndex)
			{
				if (CurrentTime < YearCircleBehaviorConfig[NextBehaviorIndex])
				{
					break;
				}
			}

			bool bIsNextCircle = NextBehaviorIndex == ConfigNum;
			NextBehaviorIndex %= ConfigNum;

			int32 BehaviorIndex = ((NextBehaviorIndex - 1) + ConfigNum) % ConfigNum;
			if (BehaviorIndex != PrevChild)
			{
				int32 Year = CurrentTime.GetYear();
				if (bIsNextCircle)
				{
					Year += 1;
				}

				int32 Month, DayOfMonth, Hour, Minute;
				YearCircleBehaviorConfig[NextBehaviorIndex].GetConfigSafe(Year, Month, DayOfMonth, Hour, Minute);

				FXD_SpecialTimeConfig NextExecuteTimeBehaviorStartTime = FXD_SpecialTimeConfig(Year, Month, DayOfMonth, Hour, Minute);
				TimeCircleMemory->Handle = TimeManager->AddNativeSpecialGameTimeEvent(NextExecuteTimeBehaviorStartTime, UXD_TimeManager::FXD_GameTimeNativeDelegate::CreateUObject(this, &UBTComposite_ARPG_HourTimeCircle::ExecuteNextTimeCircleEvent, SearchData, NextBehaviorIndex));

				AI_V_Display_LOG(SearchData.OwnerComp, "[%s][%s]", *CurrentTime.ToString(), *GetRowBehaviorDesc(BehaviorIndex));

				return BehaviorIndex;
			}
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
				Desc += FString(TEXT("\n")) + GetRowBehaviorDesc(i);
			}
		}
	}

	return Desc;
}

FString UBTComposite_ARPG_YearTimeCircle::GetRowBehaviorDesc(int32 StartIndex) const
{
	if (StartIndex == YearCircleBehaviorConfig.Num() - 1)
	{
		return FString::Printf(TEXT("%s 至下一个 %s : %s"), *YearCircleBehaviorConfig[StartIndex].ToString(), *YearCircleBehaviorConfig[0].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
	else
	{
		return FString::Printf(TEXT("%s 至 %s : %s"), *YearCircleBehaviorConfig[StartIndex].ToString(), *YearCircleBehaviorConfig[StartIndex + 1].ToString(), *GetChildNode(StartIndex)->GetNodeName());
	}
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
#endif // WITH_EDITOR
