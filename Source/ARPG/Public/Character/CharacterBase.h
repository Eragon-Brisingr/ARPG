// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XD_SaveGameInterface.h"
#include "ARPG_InputBuffer.h"
#include "ARPG_LockOnTargetSystem.h"
#include "ARPG_CharacterAnimType.h"
#include "CharacterBase.generated.h"

UCLASS()
class ARPG_API ACharacterBase : public ACharacter, 
	public IXD_SaveGameInterface,
	public IARPG_LockOnTargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//输入
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|行为")
	FARPG_InputBuffer InputBuffer;

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	void ARPG_InputPressed(EARPG_InputType InputType);

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	void ARPG_InputReleased(EARPG_InputType InputType);

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	bool ARPG_InputIsPressed(UPARAM(meta = (Bitmask, BitmaskEnum = "EARPG_InputType")) int32 InputType) const;

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	bool ARPG_InputIsReleased(UPARAM(meta = (Bitmask, BitmaskEnum = "EARPG_InputType")) int32 InputType) const;
	//移动行为
public:
	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	void InvokeChangeMoveGait(ECharacterGait Gait);

	UFUNCTION(Reliable, WithValidation, Server)
	void InvokeChangeMoveGaitToServer(const ECharacterGait& Gait);
	virtual void InvokeChangeMoveGaitToServer_Implementation(const ECharacterGait& Gait);
	bool InvokeChangeMoveGaitToServer_Validate(const ECharacterGait& Gait) { return true; }

	//IARPG_LockOnTargetInterface
public:
	UPROPERTY(EditDefaultsOnly, Category = "角色|配置")
	TArray<FName> CanLockedSocketNames;

	UPROPERTY(EditDefaultsOnly, Category = "角色|配置")
	float MaxLockingDistance = 2000.f;

	virtual FVector GetTargetLocation_Implementation(const FName& CurLockSocketName) const override;

	virtual bool CanLockedOnTarget_Implementation(AController* Invoker, const FName& InvokeLockedSocketName) const override;

	virtual bool CanLockingOnTarget_Implementation(AController* Invoker, const FName& CurLockSocketName, bool& TryLockAgain) const override;

	//播放Montage
public:
	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	float PlayMontage(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None, bool ClientMaster = false);

	UFUNCTION(Reliable, WithValidation, NetMulticast)
	void MulticastPlayMontage(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	virtual void MulticastPlayMontage_Implementation(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	bool MulticastPlayMontage_Validate(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None) { return true; }

	UFUNCTION(Reliable, WithValidation, NetMulticast)
	void MulticastPlayMontageSkipOwner(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	virtual void MulticastPlayMontageSkipOwner_Implementation(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	bool MulticastPlayMontageSkipOwner_Validate(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None) { return true; }

	UFUNCTION(BlueprintCallable, Category = "角色|行为", Reliable, WithValidation, NetMulticast)
	void StopMontage();
	virtual void StopMontage_Implementation();
	bool StopMontage_Validate() { return true; }

	UFUNCTION(Reliable, WithValidation, Server)
	void PlayMontageToServer(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	virtual void PlayMontageToServer_Implementation(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	bool PlayMontageToServer_Validate(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None) { return true; }
	
	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	void TryPlayMontage(const FARPG_MontageParameter& Montage);
	//背包相关
public:
	UFUNCTION(BlueprintCallable, Category = "角色|物品", Reliable, WithValidation, Server)
	void MoveItem(class UARPG_InventoryComponent* SourceInventory, class UARPG_InventoryComponent* TargetInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number = 1);
	virtual void MoveItem_Implementation(class UARPG_InventoryComponent* SourceInventory, class UARPG_InventoryComponent* TargetInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number = 1);
	bool MoveItem_Validate(class UARPG_InventoryComponent* SourceInventory, class UARPG_InventoryComponent* TargetInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number = 1) { return true; }

	UFUNCTION(BlueprintCallable, Category = "角色|物品", Reliable, WithValidation, Server)
	void TradeItem(class UARPG_InventoryComponent* TraderInventory, class UARPG_InventoryComponent* BuyerInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number = 1);
	virtual void TradeItem_Implementation(class UARPG_InventoryComponent* TraderInventory, class UARPG_InventoryComponent* BuyerInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number = 1);
	bool TradeItem_Validate(class UARPG_InventoryComponent* TraderInventory, class UARPG_InventoryComponent* BuyerInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number = 1) { return true; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "背包")
	class UARPG_InventoryComponent* Inventory;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class UARPG_MovementComponent* ARPG_MovementComponent;
};
