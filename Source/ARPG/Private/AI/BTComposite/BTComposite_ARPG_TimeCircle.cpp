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
};

UBTComposite_ARPG_TimeCircleBase::UBTComposite_ARPG_TimeCircleBase()
{
	bUseNodeDeactivationNotify = true;
}

void UBTComposite_ARPG_TimeCircleBase::ExecuteNextTimeCircleEvent(FBehaviorTreeSearchData SearchData, int32 NextTimeCircleEventIndex) const
{
	UBTComposite_ARPG_TimeCircleBase* TimeCircleBase = const_cast<UBTComposite_ARPG_TimeCircleBase*>(this);
	const int32 InstanceIdx = SearchData.OwnerComp.FindInstanceContainingNode(TimeCircleBase);
	SearchData.OwnerComp.RequestExecution(TimeCircleBase, InstanceIdx, TimeCircleBase, NextTimeCircleEventIndex, EBTNodeResult::Aborted);
}

void UBTComposite_ARPG_TimeCircleBase::NotifyNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const
{
	if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
	{
		FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);

		TimeManager->RemoveNativeSpecialGameTimeEvent(TimeCircleMemory->NextExecuteTimeBehaviorStartTime, this);
		AI_V_Display_LOG(SearchData.OwnerComp.GetOwner(), TEXT("[%s]时间行为循环被中断"), *GetNodeName());
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
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
		{
			bool NotInitialized = PrevChild == BTSpecialChild::NotInitialized;
			if (NotInitialized || LastResult == EBTNodeResult::InProgress)
			{
				const FXD_GameTime& CurrentTime = TimeManager->CurrentTime;
				bool bIsNextDayBehavior = true;
				int32 ExecuteIndex = NotInitialized ? 0 : PrevChild + 2;
				for (; ExecuteIndex < HourCircleBehaviorConfig.Num(); ++ExecuteIndex)
				{
					if (CurrentTime < HourCircleBehaviorConfig[ExecuteIndex])
					{
						ExecuteIndex = ExecuteIndex != 0 ? ExecuteIndex - 1 : HourCircleBehaviorConfig.Num() - 1;
						bIsNextDayBehavior = false;
						break;
					}
				}
				if (bIsNextDayBehavior)
				{
					ExecuteIndex = HourCircleBehaviorConfig.Num() - 1;
				}

				if (NotInitialized)
				{
					FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);
					FXD_SpecialTimeConfig& NextExecuteTimeBehaviorStartTime = TimeCircleMemory->NextExecuteTimeBehaviorStartTime;

					int32 NextBehaviorIndex = ExecuteIndex == HourCircleBehaviorConfig.Num() - 1 ? 0 : ExecuteIndex + 1;
					int32 Minute;
					HourCircleBehaviorConfig[NextBehaviorIndex].GetConfig(Minute);
					NextExecuteTimeBehaviorStartTime = FXD_GameTime(CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), Minute);
					if (bIsNextDayBehavior)
					{
						NextExecuteTimeBehaviorStartTime = FXD_GameTime(NextExecuteTimeBehaviorStartTime.GetTicks() + FXD_GameTimeSpan(0, 1, 0).GetTicks());
					}
					TimeManager->AddNativeSpecialGameTimeEvent(NextExecuteTimeBehaviorStartTime, UXD_TimeManager::FXD_GameTimeNativeDelegate::CreateUObject(this, &UBTComposite_ARPG_HourTimeCircle::HourBehaviorCircle, SearchData, NextBehaviorIndex));
				}

				//将ExecutionRequest.SearchEnd值设置为最大即可跳转下一个子节点
				if (NotInitialized)
				{
					UBTC_Ex::SetExecutionRequestSearchEnd(SearchData.OwnerComp);
					AI_V_Display_LOG(SearchData.OwnerComp.GetOwner(), TEXT("执行时间行为循环[%s]"), *GetRowBehaviorDesc(ExecuteIndex));
				}
				else
				{
					AI_V_Display_LOG(SearchData.OwnerComp.GetOwner(), TEXT("开始时间行为循环[%s]"), *GetRowBehaviorDesc(ExecuteIndex));
				}

				return ExecuteIndex;
			}
			//若子节点执行失败则继续向下搜索
			else if (LastResult == EBTNodeResult::Failed)
			{
				int32 ExecuteIndex = PrevChild + 1 < GetChildrenNum() ? PrevChild + 1 : 0;
				AI_V_Display_LOG(SearchData.OwnerComp.GetOwner(), TEXT("[%s]无法执行，执行下一个时间行为循环[%s]"), *GetChildNode(PrevChild)->GetNodeName(), *GetRowBehaviorDesc(ExecuteIndex));
				return ExecuteIndex;
			}
		}
	}

	return BTSpecialChild::ReturnToParent;
}

void UBTComposite_ARPG_HourTimeCircle::HourBehaviorCircle(FBehaviorTreeSearchData SearchData, int32 BehaviorIndex) const
{
	FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);
	FXD_SpecialTimeConfig& NextExecuteTimeBehaviorStartTime = TimeCircleMemory->NextExecuteTimeBehaviorStartTime;

	ExecuteNextTimeCircleEvent(SearchData, BehaviorIndex);
	AI_V_Display_LOG(SearchData.OwnerComp.GetOwner(), TEXT("执行时间行为循环[%s]"), *GetRowBehaviorDesc(BehaviorIndex));

	int32 NextBehaviorIndex = BehaviorIndex == HourCircleBehaviorConfig.Num() - 1 ? 0 : BehaviorIndex + 1;

	UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp);
	const FXD_GameTime& CurrentTime = TimeManager->CurrentTime;

	int32 Minute;
	HourCircleBehaviorConfig[NextBehaviorIndex].GetConfig(Minute);
	NextExecuteTimeBehaviorStartTime = FXD_GameTime(CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), Minute);
	if (BehaviorIndex == HourCircleBehaviorConfig.Num() - 1)
	{
		NextExecuteTimeBehaviorStartTime = FXD_GameTime(NextExecuteTimeBehaviorStartTime.GetTicks() + FXD_GameTimeSpan(0, 1, 0).GetTicks());
	}

	TimeManager->AddNativeSpecialGameTimeEvent(NextExecuteTimeBehaviorStartTime, UXD_TimeManager::FXD_GameTimeNativeDelegate::CreateUObject(this, &UBTComposite_ARPG_HourTimeCircle::HourBehaviorCircle, SearchData, NextBehaviorIndex));
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
#endif // WITH_EDITOR


UBTComposite_ARPG_DayTimeCircle::UBTComposite_ARPG_DayTimeCircle()
{
	NodeName = TEXT("每日循环");
}

int32 UBTComposite_ARPG_DayTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (DayCircleBehaviorConfig.Num() > 0)
	{
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
		{
			bool NotInitialized = PrevChild == BTSpecialChild::NotInitialized;
			if (NotInitialized || LastResult == EBTNodeResult::InProgress)
			{
				const FXD_GameTime& CurrentTime = TimeManager->CurrentTime;
				bool bIsNextDayBehavior = true;
				int32 ExecuteIndex = NotInitialized ? 0 : PrevChild + 2;
				for (; ExecuteIndex < DayCircleBehaviorConfig.Num(); ++ExecuteIndex)
				{
					if (CurrentTime < DayCircleBehaviorConfig[ExecuteIndex])
					{
						ExecuteIndex = ExecuteIndex != 0 ? ExecuteIndex - 1 : DayCircleBehaviorConfig.Num() - 1;
						bIsNextDayBehavior = false;
						break;
					}
				}
				if (bIsNextDayBehavior)
				{
					ExecuteIndex = DayCircleBehaviorConfig.Num() - 1;
				}

				if (NotInitialized)
				{
					FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);
					FXD_SpecialTimeConfig& NextExecuteTimeBehaviorStartTime = TimeCircleMemory->NextExecuteTimeBehaviorStartTime;

					int32 NextBehaviorIndex = ExecuteIndex == DayCircleBehaviorConfig.Num() - 1 ? 0 : ExecuteIndex + 1;
					int32 Hour, Minute;
					DayCircleBehaviorConfig[NextBehaviorIndex].GetConfig(Hour, Minute);
					NextExecuteTimeBehaviorStartTime = FXD_GameTime(CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), Hour, Minute);
					if (bIsNextDayBehavior)
					{
						NextExecuteTimeBehaviorStartTime = FXD_GameTime(NextExecuteTimeBehaviorStartTime.GetTicks() + FXD_GameTimeSpan(1, 0, 0).GetTicks());
					}
					TimeManager->AddNativeSpecialGameTimeEvent(NextExecuteTimeBehaviorStartTime, UXD_TimeManager::FXD_GameTimeNativeDelegate::CreateUObject(this, &UBTComposite_ARPG_DayTimeCircle::DayBehaviorCircle, SearchData, NextBehaviorIndex));
				}

				//将ExecutionRequest.SearchEnd值设置为最大即可跳转下一个子节点
				if (NotInitialized)
				{
					UBTC_Ex::SetExecutionRequestSearchEnd(SearchData.OwnerComp);
					AI_V_Display_LOG(SearchData.OwnerComp.GetOwner(), TEXT("执行时间行为循环[%s]"), *GetRowBehaviorDesc(ExecuteIndex));
				}
				else
				{
					AI_V_Display_LOG(SearchData.OwnerComp.GetOwner(), TEXT("开始时间行为循环[%s]"), *GetRowBehaviorDesc(ExecuteIndex));
				}

				return ExecuteIndex;
			}
			//若子节点执行失败则继续向下搜索
			else if (LastResult == EBTNodeResult::Failed)
			{
				int32 ExecuteIndex = PrevChild + 1 < GetChildrenNum() ? PrevChild + 1 : 0;
				AI_V_Display_LOG(SearchData.OwnerComp.GetOwner(), TEXT("[%s]无法执行，执行下一个时间行为循环[%s]"), *GetChildNode(PrevChild)->GetNodeName(), *GetRowBehaviorDesc(ExecuteIndex));
				return ExecuteIndex;
			}
		}
	}

	return BTSpecialChild::ReturnToParent;
}

void UBTComposite_ARPG_DayTimeCircle::DayBehaviorCircle(FBehaviorTreeSearchData SearchData, int32 BehaviorIndex) const
{
	FTimeCircleMemory* TimeCircleMemory = GetNodeMemory<FTimeCircleMemory>(SearchData);
	FXD_SpecialTimeConfig& NextExecuteTimeBehaviorStartTime = TimeCircleMemory->NextExecuteTimeBehaviorStartTime;

	ExecuteNextTimeCircleEvent(SearchData, BehaviorIndex);
	AI_V_Display_LOG(SearchData.OwnerComp.GetOwner(), TEXT("执行时间行为循环[%s]"), *GetRowBehaviorDesc(BehaviorIndex));

	int32 NextBehaviorIndex = BehaviorIndex == DayCircleBehaviorConfig.Num() - 1 ? 0 : BehaviorIndex + 1;

	UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp);
	const FXD_GameTime& CurrentTime = TimeManager->CurrentTime;

	int32 Hour, Minute;
	DayCircleBehaviorConfig[NextBehaviorIndex].GetConfig(Hour, Minute);
	NextExecuteTimeBehaviorStartTime = FXD_GameTime(CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), Hour, Minute);
	if (BehaviorIndex == DayCircleBehaviorConfig.Num() - 1)
	{
		NextExecuteTimeBehaviorStartTime = FXD_GameTime(NextExecuteTimeBehaviorStartTime.GetTicks() + FXD_GameTimeSpan(1, 0, 0).GetTicks());
	}

	TimeManager->AddNativeSpecialGameTimeEvent(NextExecuteTimeBehaviorStartTime, UXD_TimeManager::FXD_GameTimeNativeDelegate::CreateUObject(this, &UBTComposite_ARPG_DayTimeCircle::DayBehaviorCircle, SearchData, NextBehaviorIndex));
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
	if (WeekCircleBehaviorConfig.Num() > 0 && PrevChild == BTSpecialChild::NotInitialized)
	{
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
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
#endif // WITH_EDITOR

UBTComposite_ARPG_MonthTimeCircle::UBTComposite_ARPG_MonthTimeCircle()
{
	NodeName = TEXT("每月循环");
}

int32 UBTComposite_ARPG_MonthTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (MonthCircleBehaviorConfig.Num() > 0 && PrevChild == BTSpecialChild::NotInitialized)
	{
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
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
#endif // WITH_EDITOR


UBTComposite_ARPG_YearTimeCircle::UBTComposite_ARPG_YearTimeCircle()
{
	NodeName = TEXT("每年循环");
}

int32 UBTComposite_ARPG_YearTimeCircle::GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	if (YearCircleBehaviorConfig.Num() > 0 && PrevChild == BTSpecialChild::NotInitialized)
	{
		if (UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(&SearchData.OwnerComp))
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
#endif // WITH_EDITOR
