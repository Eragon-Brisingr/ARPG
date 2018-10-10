// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XD_SaveGameInterface.h"
#include "ARPG_InputBuffer.h"
#include "ARPG_LockOnTargetSystem.h"
#include "ARPG_CharacterAnimType.h"
#include "ItemTypeUtils.h"
#include "XD_ItemType.h"
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

public:
	virtual void WhenGameInit_Implementation() override;

	virtual TArray<struct FXD_Item> GetInitItemList() const;
	UFUNCTION(BlueprintImplementableEvent, Category = "角色|初始化")
	TArray<struct FXD_Item> ReceivedGetInitItemList() const;

	//重生用
public:
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "角色|重生")
	FIntVector BornWorldOrigin;

	UPROPERTY(SaveGame, BlueprintGetter = GetRebornLocation, BlueprintSetter = SetRebornLocation, Category = "角色|重生")
	FVector BornLocation;

	UFUNCTION(BlueprintSetter)
	void SetRebornLocation(const FVector& RebornLocation);

	UFUNCTION(BlueprintGetter)
	FVector GetRebornLocation();

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
	float PlayMontage(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None, bool ClientMaster = true);

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

	UPROPERTY(EditDefaultsOnly, Category = "角色|行为")
	FName FullBodySlotName = TEXT("FullBody");

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	bool CanPlayFullBodyMontage() const;

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

	//使用道具相关
public:
	UFUNCTION(Reliable, WithValidation, Server)
	void InvokeUseItem(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput);
	virtual void InvokeUseItem_Implementation(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput);
	bool InvokeUseItem_Validate(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput) { return true; }

	UFUNCTION(BlueprintCallable, Category = "角色|物品", BlueprintAuthorityOnly)
	void UseItemImmediately(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput);

	UFUNCTION(BlueprintNativeEvent, Category = "角色|物品")
	class AARPG_WeaponBase* EquipWaepon(class UARPG_WeaponCoreBase* WeaponCore, EUseItemInput UseItemInput);
	virtual class AARPG_WeaponBase* EquipWaepon_Implementation(class UARPG_WeaponCoreBase* WeaponCore, EUseItemInput UseItemInput) { return nullptr; }

	UFUNCTION(BlueprintNativeEvent, Category = "角色|物品")
	class AARPG_EquipmentBase* EquipEquipment(class UARPG_EquipmentCoreBase* EquipmentCore, EUseItemInput UseItemInput);
	virtual class AARPG_EquipmentBase* EquipEquipment_Implementation(class UARPG_EquipmentCoreBase* EquipmentCore, EUseItemInput UseItemInput) { return nullptr; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUseItem, ACharacterBase*, Character, const class UARPG_ItemCoreBase*, ItemCore, EUseItemInput, UseItemInput);
	UPROPERTY(BlueprintAssignable)
	FOnUseItem OnUseItem;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquip, ACharacterBase*, Character, class AARPG_ItemBase*, EquipItem);
	UPROPERTY(BlueprintAssignable, Category = "角色|物品")
	FOnEquip OnEquip;
	 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNotEquip, ACharacterBase*, Character, class AARPG_ItemBase*, NotEquipItem);
	UPROPERTY(BlueprintAssignable, Category = "角色|物品")
	FOnNotEquip OnNotEquip;

	//战斗相关
public:
	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	void NearAttackSuccessTimeDilation(float Duration);
	static constexpr float AttackSuccessTimeDilationRate = 0.01f;
	FTimerHandle AttackSuccessTimeDilation_TimeHandle;
	void RecoverAttackSuccessTimeDilation();

	//防御
	UPROPERTY(BlueprintReadWrite, Category = "角色|行为")
	uint8 bIsDefense : 1;
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "角色|行为")
	bool IsDefenseSucceed(const FVector& DamageFromLocation, const FHitResult& HitInfo) const;
	virtual bool IsDefenseSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const;

	//防御反击
	UPROPERTY(BlueprintReadWrite, Category = "角色|行为")
	uint8 bIsDefenseSwipe : 1;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "角色|行为")
	bool IsDefenseSwipeSucceed(const FVector& DamageFromLocation, const FHitResult& HitInfo) const;
	virtual bool IsDefenseSwipeSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class UARPG_MovementComponent* ARPG_MovementComponent;
};
