// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_BattleStyleSystemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, collapsecategories, abstract)
class ARPG_API UARPG_BattleStyleSystemBase : public UObject
{
	GENERATED_BODY()
public:
	virtual class UWorld* GetWorld() const override;

	UPROPERTY(BlueprintReadWrite, Category = "战斗风格系统")
	class ACharacterBase* Character;

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统")
	class UAIPerceptionComponent* GetPerceptionComponent() const;

	void InitBattleStyleSystem(class ACharacterBase* Owner)
	{
		Character = Owner;
		WhenInitBattleStyleSystem();
	}
	virtual void WhenInitBattleStyleSystem() { ReceiveWhenBattleStyleSystem(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统", meta = (DisplayName = "WhenInitBattleStyleSystem"))
	void ReceiveWhenBattleStyleSystem();

	//状态切换
public:
	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|状态切换")
	virtual void WhenEnterAlertState() { ReceiveWhenEnterAlertState(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|状态切换", meta = (DisplayName = "WhenEnterAlertState"))
	void ReceiveWhenEnterAlertState();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|状态切换")
	virtual void WhenLeaveAlertState() { ReceiveWhenLeaveAlertState(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|状态切换", meta = (DisplayName = "WhenLeaveAlertState"))
	void ReceiveWhenLeaveAlertState();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|状态切换")
	virtual void WhenEnterBattleState() { ReceiveWhenEnterBattleState(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|状态切换", meta = (DisplayName = "WhenEnterBattleState"))
	void ReceiveWhenEnterBattleState();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|状态切换")
	virtual void WhenLeaveBattleState() { ReceiveWhenLeaveBattleState(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|状态切换", meta = (DisplayName = "WhenLeaveBattleState"))
	void ReceiveWhenLeaveBattleState();

	//意识
public:
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "战斗风格系统|意识")
	virtual FVector GetBestBattleLocation(class AActor* Enemy) const { return ReceiveGetBestBattleLocation(Enemy); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|意识", meta = (DisplayName = "GetBestBattleLocation"))
	FVector ReceiveGetBestBattleLocation(class AActor* Enemy) const;

	//PotentialLocation可为InvalidLocation
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "战斗风格系统|意识")
	virtual FVector GetBestSearchLocation(const FVector& PotentialLocation) const { return ReceiveGetBestSearchLocation(PotentialLocation); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|意识", meta = (DisplayName = "GetBestSearchLocation"))
	FVector ReceiveGetBestSearchLocation(const FVector& PotentialLocation) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "战斗风格系统|意识")
	bool CanAttack(class AActor* AttackTarget) const { return ReceiveCanAttack(AttackTarget); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|意识", meta = (DisplayName = "CanAttack"))
	bool ReceiveCanAttack(class AActor* Target) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "战斗风格系统|意识")
	bool CanDefense(class AActor* DefenseTarget) const { return ReceiveCanDefense(DefenseTarget); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|意识", meta = (DisplayName = "CanDefense"))
	bool ReceiveCanDefense(class AActor* DefenseTarget) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "战斗风格系统|意识")
	bool IsInDangerousArea() const { return ReceiveIsInDangerousArea(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|意识", meta = (DisplayName = "IsInDangerousArea"))
	bool ReceiveIsInDangerousArea() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "战斗风格系统|意识")
	bool Tick(float DeltaTime) const { return ReceiveTick(DeltaTime); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|意识", meta = (DisplayName = "Tick"))
	bool ReceiveTick(float DeltaTime) const;

	//行为
public:
	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|行为")
	void InvokeAttackStart() { ReceiveInvokeAttackStart(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|行为", meta = (DisplayName = "InvokeAttackStart"))
	void ReceiveInvokeAttackStart();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|行为")
	void InvokeAttackEnd() { ReceiveInvokeAttackEnd(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|行为", meta = (DisplayName = "InvokeAttackEnd"))
	void ReceiveInvokeAttackEnd();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|行为")
	void InvokeAttackAbort() { ReceiveInvokeAttackAbort(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|行为", meta = (DisplayName = "InvokeAttackAbort"))
	void ReceiveInvokeAttackAbort();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|行为")
	void InvokeDefenseStart() { ReceiveInvokeDefenseStart(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|行为", meta = (DisplayName = "InvokeDefenseStart"))
	void ReceiveInvokeDefenseStart();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|行为")
	void InvokeDefenseEnd() { ReceiveInvokeDefenseEnd(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|行为", meta = (DisplayName = "InvokeDefenseEnd"))
	void ReceiveInvokeDefenseEnd();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|行为")
	void InvokeDefenseAbort() { ReceiveInvokeDefenseAbort(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|行为", meta = (DisplayName = "InvokeDefenseAbort"))
	void ReceiveInvokeDefenseAbort();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|行为")
	void InvokeDodgeStart() { ReceiveInvokeDodgeStart(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|行为", meta = (DisplayName = "InvokeDodgeStart"))
	void ReceiveInvokeDodgeStart();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|行为")
	void InvokeDodgeEnd() { ReceiveInvokeDodgeEnd(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|行为", meta = (DisplayName = "InvokeDodgeEnd"))
	void ReceiveInvokeDodgeEnd();

	UFUNCTION(BlueprintCallable, Category = "战斗风格系统|行为")
	void InvokeDodgeAbort() { ReceiveInvokeDodgeAbort(); }
	UFUNCTION(BlueprintImplementableEvent, Category = "战斗风格系统|行为", meta = (DisplayName = "InvokeDodgeAbort"))
	void ReceiveInvokeDodgeAbort();
};
