// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARPG_InteractInterface.generated.h"

class ACharacterBase;

UENUM()
enum class EInteractEndResult : uint8
{
	CanNotInteract,
	Succeed,
	Failed
};

DECLARE_DELEGATE_OneParam(FOnInteractEnd, EInteractEndResult);

USTRUCT(BlueprintType, BlueprintInternalUseOnly)
struct ARPG_API FOnInteractEndEvent
{
	GENERATED_BODY()
public:
	FOnInteractEndEvent() = default;
	FOnInteractEndEvent(const FOnInteractEnd& OnInteractEnd)
		:OnInteractEnd(OnInteractEnd)
	{}

	FOnInteractEnd OnInteractEnd;

	void ExecuteIfBound(EInteractEndResult Result) const { OnInteractEnd.ExecuteIfBound(Result); }
};

DECLARE_DELEGATE(FOnInteractAbortEnd);
USTRUCT(BlueprintType, BlueprintInternalUseOnly)
struct ARPG_API FOnInteractAbortEndEvent
{
	GENERATED_BODY()
public:
	FOnInteractAbortEndEvent() = default;
	FOnInteractAbortEndEvent(const FOnInteractAbortEnd& OnInteractAbortEnd)
		:OnInteractAbortEnd(OnInteractAbortEnd)
	{}

	FOnInteractAbortEnd OnInteractAbortEnd;

	void ExecuteIfBound() const { OnInteractAbortEnd.ExecuteIfBound(); }
};

UCLASS()
class ARPG_API UARPG_InteractLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void ExecuteInteractEndEvent(const FOnInteractEndEvent& Event, EInteractEndResult Result) { Event.ExecuteIfBound(Result); }

	UFUNCTION()
	void ExecuteInteractAbortEndEvent(const FOnInteractAbortEndEvent& Event) { Event.ExecuteIfBound(); }
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UARPG_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_API IARPG_InteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//申请交互 e.g.移动至交互位置
	UFUNCTION(BlueprintNativeEvent, Category = "游戏|交互")
	void WhenInvokeInteract(ACharacterBase* InteractInvoker, const FOnInteractEndEvent& Event);
	virtual void WhenInvokeInteract_Implementation(ACharacterBase* InteractInvoker, const FOnInteractEndEvent& Event);
	static void WhenInvokeInteract(UObject* Obj, ACharacterBase* InteractInvoker, const FOnInteractEndEvent& Event);

	//执行交互 e.g.道具添加入背包
	UFUNCTION(BlueprintNativeEvent, Category = "游戏|交互")
	void WhenExecuteInteract(ACharacterBase* InteractInvoker, const FOnInteractEndEvent& Event);
	virtual void WhenExecuteInteract_Implementation(ACharacterBase* InteractInvoker, const FOnInteractEndEvent& Event) {}
	static void WhenExecuteInteract(UObject* Obj, ACharacterBase* InteractInvoker, const FOnInteractEndEvent& Event) { IARPG_InteractInterface::Execute_WhenExecuteInteract(Obj, InteractInvoker, Event); }

	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	bool CanInteract(const ACharacterBase* InteractInvoker) const;
	virtual bool CanInteract_Implementation(const ACharacterBase* InteractInvoker) const { return false; }
	static bool CanInteract(UObject* Obj, const ACharacterBase* InteractInvoker) { return IARPG_InteractInterface::Execute_CanInteract(Obj, InteractInvoker); }

	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	void WhenAbortInteract(ACharacterBase* InteractInvoker, const FOnInteractAbortEndEvent& Event);
	virtual void WhenAbortInteract_Implementation(ACharacterBase* InteractInvoker, const FOnInteractAbortEndEvent& Event) {}
	static void WhenAbortInteract(UObject* Obj, ACharacterBase* InteractInvoker, const FOnInteractAbortEndEvent& Event) { IARPG_InteractInterface::Execute_WhenAbortInteract(Obj, InteractInvoker, Event); }
public:
	UFUNCTION(BlueprintNativeEvent, Category = "游戏|交互")
	void GetHintInfo(ACharacterBase* InteractInvoker);
	virtual void GetHintInfo_Implementation(ACharacterBase* InteractInvoker) {}

	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	bool CanShowHintInfo(const ACharacterBase* InteractInvoker) const;
	virtual bool CanShowHintInfo_Implementation(const ACharacterBase* InteractInvoker) const { return CanInteract(const_cast<UObject*>(Cast<UObject>(this)), InteractInvoker); }
	static bool CanShowHintInfo(UObject* Obj, ACharacterBase* InteractInvoker) { return IARPG_InteractInterface::Execute_CanShowHintInfo(Obj, InteractInvoker); }
};
