// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARPG_InteractInterface.generated.h"

class ACharacterBase;

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
	void WhenInvokeInteract(ACharacterBase* InteractInvoker);
	virtual void WhenInvokeInteract_Implementation(ACharacterBase* InteractInvoker);
	static void WhenInvokeInteract(UObject* Obj, ACharacterBase* InteractInvoker);

	//执行交互 e.g.道具添加入背包
	UFUNCTION(BlueprintNativeEvent, Category = "游戏|交互")
	void WhenExecuteInteract(ACharacterBase* InteractInvoker);
	virtual void WhenExecuteInteract_Implementation(ACharacterBase* InteractInvoker) {}
	static void WhenExecuteInteract(UObject* Obj, ACharacterBase* InteractInvoker) { IARPG_InteractInterface::Execute_WhenExecuteInteract(Obj, InteractInvoker); }

	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	bool CanInteract(const ACharacterBase* InteractInvoker) const;
	virtual bool CanInteract_Implementation(const ACharacterBase* InteractInvoker) const { return false; }
	static bool CanInteract(UObject* Obj, const ACharacterBase* InteractInvoker) { return IARPG_InteractInterface::Execute_CanInteract(Obj, InteractInvoker); }

	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	void WhenAbortInteract(ACharacterBase* InteractInvoker);
	virtual void WhenAbortInteract_Implementation(ACharacterBase* InteractInvoker) {}
	static void WhenAbortInteract(UObject* Obj, ACharacterBase* InteractInvoker) { IARPG_InteractInterface::Execute_WhenAbortInteract(Obj, InteractInvoker); }

	//正式开始交互时的事件，客户端也会触发
	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	void WhenBeginInteract(ACharacterBase* InteractInvoker);
	virtual void WhenBeginInteract_Implementation(ACharacterBase* InteractInvoker) {}
	static void WhenBeginInteract(UObject* Obj, ACharacterBase* InteractInvoker) { IARPG_InteractInterface::Execute_WhenBeginInteract(Obj, InteractInvoker); }

	//正式开始交互时的事件，客户端也会触发
	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	void WhenEndInteract(ACharacterBase* InteractInvoker);
	virtual void WhenEndInteract_Implementation(ACharacterBase* InteractInvoker) {}
	static void WhenEndInteract(UObject* Obj, ACharacterBase* InteractInvoker) { IARPG_InteractInterface::Execute_WhenEndInteract(Obj, InteractInvoker); }
public:
	UFUNCTION(BlueprintNativeEvent, Category = "游戏|交互")
	void GetHintInfo(ACharacterBase* InteractInvoker);
	virtual void GetHintInfo_Implementation(ACharacterBase* InteractInvoker) {}

	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	bool CanShowHintInfo(const ACharacterBase* InteractInvoker) const;
	virtual bool CanShowHintInfo_Implementation(const ACharacterBase* InteractInvoker) const { return CanInteract(const_cast<UObject*>(Cast<UObject>(this)), InteractInvoker); }
	static bool CanShowHintInfo(UObject* Obj, ACharacterBase* InteractInvoker) { return IARPG_InteractInterface::Execute_CanShowHintInfo(Obj, InteractInvoker); }
};
