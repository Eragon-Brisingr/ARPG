// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ARPG_HUDBase.generated.h"

class UARPG_InventoryComponent;

/**
 * 
 */
UCLASS(showcategories=(Input))
class ARPG_API AARPG_HUDBase : public AHUD
{
	GENERATED_BODY()
public:
	AARPG_HUDBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	//交互提示系统
public:
	UPROPERTY()
	float MaxInteractTraceRadius = 400.f;
	UPROPERTY()
	TArray<AActor*> ShowHintActors;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorEnableHint, AActor*, Actor);
	UPROPERTY(BlueprintAssignable)
	FOnActorEnableHint OnActorEnableHint;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDisableHint, AActor*, Actor);
	UPROPERTY(BlueprintAssignable)
	FOnActorDisableHint OnActorDisableHint;

	UFUNCTION(BlueprintCallable, Category = "交互提示系统")
	AActor* GetNearestShowHintActor() const;

	//背包系统
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "面板事件")
	void OpenMoveItemPanel(UARPG_InventoryComponent* OtherInventory);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "面板事件")
	void CloseMoveItemPanel();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "面板事件")
	void OpenTradeItemPanel(UARPG_InventoryComponent* OtherInventory);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "面板事件")
	void CloseTradeItemPanel();
};
