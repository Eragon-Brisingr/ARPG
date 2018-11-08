// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARPG_InteractInterface.generated.h"

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
	void WhenInvokeInteract(class ACharacterBase* InteractInvoker);
	virtual void WhenInvokeInteract_Implementation(class ACharacterBase* InteractInvoker);
	static void WhenInvokeInteract(UObject* Obj, class ACharacterBase* InteractInvoker);

	//执行交互 e.g.道具添加入背包
	UFUNCTION(BlueprintNativeEvent, Category = "游戏|交互")
	void WhenExecuteInteract(class ACharacterBase* InteractInvoker);
	virtual void WhenExecuteInteract_Implementation(class ACharacterBase* InteractInvoker) {}
	static void WhenExecuteInteract(UObject* Obj, class ACharacterBase* InteractInvoker) { IARPG_InteractInterface::Execute_WhenExecuteInteract(Obj, InteractInvoker); }

	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	bool CanInteract(const class ACharacterBase* InteractInvoker) const;
	virtual bool CanInteract_Implementation(const class ACharacterBase* InteractInvoker) const { return false; }
	static bool CanInteract(UObject* Obj, const class ACharacterBase* InteractInvoker) { return IARPG_InteractInterface::Execute_CanInteract(Obj, InteractInvoker); }
public:
	UFUNCTION(BlueprintNativeEvent, Category = "游戏|交互")
	void GetHintInfo(class ACharacterBase* InteractInvoker);
	virtual void GetHintInfo_Implementation(class ACharacterBase* InteractInvoker) {}

	UFUNCTION(BlueprintNativeEvent, Category = "环境|交互")
	bool CanShowHintInfo(const class ACharacterBase* InteractInvoker) const;
	virtual bool CanShowHintInfo_Implementation(const class ACharacterBase* InteractInvoker) const { return CanInteract(const_cast<UObject*>(Cast<UObject>(this)), InteractInvoker); }
	static bool CanShowHintInfo(UObject* Obj, class ACharacterBase* InteractInvoker) { return IARPG_InteractInterface::Execute_CanShowHintInfo(Obj, InteractInvoker); }
};
