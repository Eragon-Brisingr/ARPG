// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Perception/AISightTargetInterface.h>
#include <GenericTeamAgentInterface.h>
#include "XD_SaveGameInterface.h"
#include "ARPG_InputBuffer.h"
#include "ARPG_LockOnTargetSystem.h"
#include "ARPG_CharacterAnimType.h"
#include "ItemTypeUtils.h"
#include "CharacterDamageType.h"
#include "ARPG_InteractInterface.h"
#include "ARPG_CollisionType.h"
#include "ARPG_CampType.h"
#include "ARPG_BattleType.h"
#include "BehaviorTreeEx.h"
#include "CharacterBase.generated.h"

UCLASS()
class ARPG_API ACharacterBase : public ACharacter, 
	public IXD_SaveGameInterface,
	public IARPG_LockOnTargetInterface,
	public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual bool NeedNotSave_Implementation() const override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
public:
	virtual void WhenGameInit_Implementation() override;

	virtual TArray<struct FARPG_Item> GetInitItemList() const;
	UFUNCTION(BlueprintImplementableEvent, Category = "角色|初始化", meta = (DisplayName = "GetInitItemList"))
	TArray<struct FARPG_Item> ReceivedGetInitItemList() const;

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

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	bool ARPG_AnyInputIsPressed(UPARAM(meta = (Bitmask, BitmaskEnum = "EARPG_InputType")) int32 InputType) const;

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	bool ARPG_AllInputIsReleased(UPARAM(meta = (Bitmask, BitmaskEnum = "EARPG_InputType")) int32 InputType) const;
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

	UPROPERTY(Replicated)
	uint8 bIsLockingOther : 1;

	virtual void SetLockedTarget(AActor* NewLockedTarget)
	{
		bIsLockingOther = NewLockedTarget ? true : false;
	}
	//动画
public:
	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	float PlayMontage(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None, bool ClientMaster = true);

	float PlayMontageImpl(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	UFUNCTION(Reliable, WithValidation, NetMulticast)
	void MulticastPlayMontage(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	virtual void MulticastPlayMontage_Implementation(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	bool MulticastPlayMontage_Validate(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None) { return true; }

	UFUNCTION(Reliable, WithValidation, NetMulticast)
	void MulticastPlayMontageSkipOwner(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	virtual void MulticastPlayMontageSkipOwner_Implementation(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	bool MulticastPlayMontageSkipOwner_Validate(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None) { return true; }

	UFUNCTION(BlueprintCallable, Category = "角色|行为", Reliable, WithValidation, NetMulticast)
	void StopMontage(UAnimMontage* MontageToStop = nullptr);
	virtual void StopMontage_Implementation(UAnimMontage* MontageToStop = nullptr);
	bool StopMontage_Validate(UAnimMontage* MontageToStop = nullptr) { return true; }

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "角色", AdvancedDisplay)
	class UAimOffsetBlendSpace* AimOffsetOverride;

	//闪避
	UPROPERTY(VisibleDefaultsOnly, Category = "角色|动画配置", Instanced)
	class UARPG_DodgeAnimSetBase* DodgeAnimSet;
	
	UFUNCTION(BlueprintCallable, Category = "角色|动画")
	void InvokeDodge();

	void DodgeByControlRotation(float Direction);

	UFUNCTION(BlueprintCallable, Category = "角色|动画")
	void InvokeDodgeByDirection(EDodgeDirection Direction);

	UFUNCTION(BlueprintCallable, Category = "角色|动画")
	bool CanDodge() const;
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
	UFUNCTION(BlueprintCallable, Category = "角色|物品", Reliable, WithValidation, Server)
	void InvokeUseItem(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput);
	virtual void InvokeUseItem_Implementation(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput);
	bool InvokeUseItem_Validate(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput) { return true; }

	UFUNCTION(BlueprintCallable, Category = "角色|物品", BlueprintAuthorityOnly)
	void UseItemImmediately(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput = EUseItemInput::LeftMouse);

	UFUNCTION(BlueprintNativeEvent, Category = "角色|物品")
	class AARPG_WeaponBase* EquipWaepon(class UARPG_WeaponCoreBase* WeaponCore, EUseItemInput UseItemInput);
	virtual class AARPG_WeaponBase* EquipWaepon_Implementation(class UARPG_WeaponCoreBase* WeaponCore, EUseItemInput UseItemInput) { return nullptr; }
	
	UFUNCTION(BlueprintNativeEvent, Category = "角色|物品")
	class AARPG_ArrowBase* EquipArrow(class UARPG_ArrowCoreBase* ArrowCore, EUseItemInput UseItemInput);
	virtual class AARPG_ArrowBase* EquipArrow_Implementation(class UARPG_ArrowCoreBase* ArrowCore, EUseItemInput UseItemInput) { return nullptr; }

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
	//卡刀效果
	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	void NearAttackSuccessTimeDilation(float Duration);
	static constexpr float AttackSuccessTimeDilationRate = 0.01f;
	FTimerHandle AttackSuccessTimeDilation_TimeHandle;
	void RecoverAttackSuccessTimeDilation();

	//闪避
	UPROPERTY(BlueprintReadOnly, Category = "角色|行为")
	uint8 bIsDodging : 1;

	//闪避
	virtual void WhenDodgeSucceed(float BaseDamage, class ACharacterBase* InstigatedBy, const FHitResult& HitResult);
	UFUNCTION(BlueprintImplementableEvent, Category = "角色|行为")
	void ReceiveWhenDodgeSucceed(float BaseDamage, class ACharacterBase* InstigatedBy, const FHitResult& HitResult);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDodgeSucceed, ACharacterBase*, Character, float, BaseDamage, class ACharacterBase*, InstigatedBy, const FHitResult&, HitResult);
	UPROPERTY(BlueprintAssignable, Category = "角色|事件")
	FOnDodgeSucceed OnDodgeSucceed;

	//防御
	UPROPERTY(BlueprintReadWrite, Category = "角色|行为")
	uint8 bIsDefense : 1;
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "角色|行为")
	bool IsDefenseSucceed(const FVector& DamageFromLocation, const FHitResult& HitInfo) const;
	virtual bool IsDefenseSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const;

	void WhenDefenseSucceed(float BaseDamage, class ACharacterBase* InstigatedBy, const FVector& HitFromDirection, float DefenseBeakBackDistance, const FHitResult& HitResult);
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "角色|行为")
	void ReceiveWhenDefenseSucceed(float BaseDamage, class ACharacterBase* InstigatedBy, const FVector& HitFromDirection, float DefenseBeakBackDistance, const FHitResult& HitResult);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDefenseSucceed, ACharacterBase*, Character, float, BaseDamage, class ACharacterBase*, InstigatedBy, const FHitResult&, HitResult);
	UPROPERTY(BlueprintAssignable, Category = "角色|事件")
	FOnDefenseSucceed OnDefenseSucceed;

	void WhenAttackedDefenseCharacter(float BaseDamage, ACharacterBase* DefenseSucceedCharacter, const FHitResult& HitResult);
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "角色|行为")
	void ReceiveWhenAttackedDefenseCharacter(float BaseDamage, ACharacterBase* DefenseSucceedCharacter, const FHitResult& HitResult);

	//防御反击
	UPROPERTY(BlueprintReadWrite, Category = "角色|行为")
	uint8 bIsDefenseSwipe : 1;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "角色|行为")
	bool IsDefenseSwipeSucceed(const FVector& DamageFromLocation, const FHitResult& HitInfo) const;
	virtual bool IsDefenseSwipeSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const;

	//强韧度，当硬直积累量高于该值则播放硬直动画
	//只可加减
	float ToughnessValue = 50.f;

	//削韧积累值
	UPROPERTY(BlueprintReadOnly, Category = "角色|战斗")
	float HitStunValue;

	UPROPERTY(EditDefaultsOnly, Category = "角色|配置")
	float HitStunClearTime = 30.f;

	//返回削韧溢出数值，若没溢出则返回-1
	float AddHitStun(float Value);
	FTimerHandle ClearHitStun_TimeHandle;
	void ClearHitStun();

	UPROPERTY()
	TMap<class UAnimNotifyState*, class USocketMoveTracer*> SocketMoveTracerMap;

	//处决
public:
	//处决受击方
	UPROPERTY(BlueprintReadWrite, Category = "游戏|行为")
	ACharacterBase* ExecuteTargetCharacter;
	//处决攻击方
	UPROPERTY(BlueprintReadWrite, Category = "角色|行为")
	ACharacterBase* ExecuteFromCharacter;

	//背刺
	bool CanBeBackstab(ACharacterBase* BackstabInvoker) const;

	void ExecuteOther(ACharacterBase* ExecuteTarget, const FVector& TargetLocation, const FRotator& TargetRotation, UAnimMontage* ExecuteMontage, UAnimMontage* BeExecutedMontage);
	UFUNCTION(Reliable, WithValidation, Server)
	void ExecuteOtherToServer(ACharacterBase* ExecuteTarget, const FVector& TargetLocation, const FRotator& TargetRotation, UAnimMontage* ExecuteMontage, UAnimMontage* BeExecutedMontage);
	void ExecuteOtherToServer_Implementation(ACharacterBase* ExecuteTarget, const FVector& TargetLocation, const FRotator& TargetRotation, UAnimMontage* ExecuteMontage, UAnimMontage* BeExecutedMontage);
	bool ExecuteOtherToServer_Validate(ACharacterBase* ExecuteTarget, const FVector& TargetLocation, const FRotator& TargetRotation, UAnimMontage* ExecuteMontage, UAnimMontage* BeExecutedMontage) { return true; }

	//通知
public:
	virtual void WhenKillOther(ACharacterBase* WhoBeKilled, UObject* KillInstigator);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnKillOther, ACharacterBase*, Killer, ACharacterBase*, WhoBeKilled, UObject*, KillInstigator);
	UPROPERTY(BlueprintAssignable, Category = "角色|事件")
	FOnKillOther OnKillOther;
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, meta = (DisplayName = "WhenDamagedOther"), Category = "角色|行为")
	void ReceiveWhenKillOther(ACharacterBase* WhoBeKilled, UObject* KillInstigator);

	virtual void WhenDamagedOther(ACharacterBase* WhoBeDamaged, float DamageValue, UObject* DamageInstigator);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDamagedOther, ACharacterBase*, Attacker, ACharacterBase*, WhoBeDamaged, float, DamageValue, UObject*, DamageInstigator);
	UPROPERTY(BlueprintAssignable, Category = "角色|事件")
	FOnDamagedOther OnDamagedOther;
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, meta = (DisplayName = "WhenDamagedOther"), Category = "角色|行为")
	void ReceiveWhenDamagedOther(ACharacterBase* WhoBeDamaged, float DamageValue, UObject* DamageInstigator);

	float ApplyPointDamage(float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitInfo, class ACharacterBase* InstigatorBy, AActor* DamageCauser, TSubclassOf<class UDamageType> DamageTypeClass, const FApplyPointDamageParameter& Param);

	//潜行相关
public:
	UFUNCTION(BlueprintCallable, Category = "角色|状态")
	virtual bool IsSneaking() const;

	//AI战斗相关
public:
	TArray<FARPG_AttackInfo> AttackInfos;

	void AddAttackInfo(const FARPG_AttackInfo& AttackInfo) { AttackInfos.Add(AttackInfo); }

	void RemoveAttackInfo(const FARPG_AttackInfo& AttackInfo) { AttackInfos.RemoveSingle(AttackInfo); }

	//播放动画相关
public:
	//硬直动画接口，会打断攻击行为
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "角色|行为", meta = (DisplayName = "PlayHitStunMontage"))
	void ReceivePlayHitStunMontage(float BaseDamage, float HitStunOverflowValue, const FVector& HitFromDirection, const FHitResult& HitResult, class ACharacterBase* InstigatedBy, AActor* DamageCauser);

	//普通状态下的受击动画接口，不会打断攻击行为
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "角色|行为", meta = (DisplayName = "PlayNormalDamageMontage"))
	void ReceivePlayNormalDamageMontage(float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitResult, class ACharacterBase* InstigatedBy, AActor* DamageCauser);

	//交互相关
public:
	UFUNCTION(BlueprintCallable, Category = "角色|交互")
	void InvokeInteract(AActor* InteractTarget);
	UFUNCTION(Reliable, WithValidation, Server)
	void InvokeInteract_ToServer(AActor* InteractTarget);
	void InvokeInteract_ToServer_Implementation(AActor* InteractTarget);
	bool InvokeInteract_ToServer_Validate(AActor* InteractTarget) { return true; }

	UFUNCTION(BlueprintCallable, Category = "角色|交互")
	bool CanInteract(AActor* InteractTarget) const;
public:
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	class UARPG_MovementComponent* ARPG_MovementComponent;

	//关系相关
public:
	//阵营关系

	UPROPERTY(EditAnywhere, Category = "角色|配置|阵营")
	FARPG_CampConfig CampConfig;

	UFUNCTION(BlueprintCallable, Category = "角色|阵营")
	class UARPG_CampInfo* GetCampInfo() const;

	UFUNCTION(BlueprintCallable, Category = "角色|阵营")
	class UARPG_CampRelationship* GetCampRelationshipToward(ACharacterBase* Other) const;

	ETeamAttitude::Type GetRelationshipTowards(const AActor* Actor) const;


	//总关系

	UFUNCTION(BlueprintCallable, Category = "角色|关系")
	ECharacterRelationship GetRelationshipTowards(const ACharacterBase* Towards) const;

	//AI
public:
	//感知

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "角色|AI|配置")
	TEnumAsByte<ECollisionChannel> SightCollisionChannel = FARPG_ECollisionChannel::AI_Sight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "角色|AI|配置")
	FName HeadSocketName = TEXT("HeadSightSocket");

	//使用头部骨骼的方向作为视线方向
	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;
	//AISightTargetInterface Start
	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = NULL) const override;
	//AISightTargetInterface End

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "角色|AI|配置")
	TSubclassOf<class UNavigationQueryFilter> NavigationQueryFilter;

	//接收到寻路请求时的处理
	void WhenReceivedMoveRequest();

	//子系统

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|AI|配置", Instanced)
	class UARPG_HatredControlSystemBase* HatredControlSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|AI|配置", Instanced)
	class UARPG_BattleStyleSystemBase* BattleStyleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|AI|配置", Instanced)
	class UARPG_AlertSystemBase* AlertSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|AI|配置", Instanced)
	class UARPG_SneakSystemBase* SneakSystem;

	//意识

	//警戒值
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "角色|AI")
	float AlertValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "角色|AI|配置")
	float AlertEntirelyValue = 100.f;
	UFUNCTION(BlueprintCallable, Category = "角色|AI")
	void AddAlertValue(float AddValue);
	UFUNCTION(BlueprintCallable, Category = "角色|AI")
	void ReduceAlertValue(float ReduceValue);

	UPROPERTY(EditDefaultsOnly, Category = "角色|AI|配置")
	float AlertSubsidedSpeed = 5.f;
	FTimerHandle AlertSubsided_TimerHandle;
	void AlertSubsided();

	UPROPERTY(EditDefaultsOnly, Category = "角色|AI|配置")
	float AlertNotEntirelySubsidedDelay = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "角色|AI|配置")
	float AlertEntirelySubsidedDelay = 10.f;

	UFUNCTION(BlueprintCallable, Category = "角色|AI")
	EAlertState GetAlertState() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|AI|配置")
	ECharacterRelationship MaxAlertRelationship = ECharacterRelationship::Hostile;

	//思考逻辑

	UPROPERTY(EditAnywhere, Category = "角色|AI|配置")
	FBehaviorTreeWithSubTree MainBehaviorTree;
};
