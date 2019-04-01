// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "ARPG_InteractType.h"
#include "ARPG_CharacterBehaviorType.h"
#include "GameplayTagContainer.h"
#include "ARPG_InteractableActorManager.generated.h"

class ACharacterBase;
class UARPG_AD_InteractableBase;
class UARPG_CharacterBehaviorBase;

USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = true))
struct FInteractBehavior : public FBehaviorPositionData
{
	GENERATED_BODY()

	//e.g.电梯的情况
	UPROPERTY(EditAnywhere, Category = "行为")
	uint8 bAttachToActor : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "行为", Instanced)
	UARPG_CharacterBehaviorBase* Behavior = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "行为", Instanced)
	UARPG_AD_InteractableBase* InteractAction = nullptr;
public:
	FVector GetWorldLocation(const UActorComponent* Component) const { return Component->GetOwner()->GetActorTransform().TransformPosition(Location); }
	FRotator GetWorldRotation(const UActorComponent* Component) const { return Component->GetOwner()->GetActorTransform().TransformRotation(Rotation.Quaternion()).Rotator(); }
};

USTRUCT(BlueprintType)
struct FInteractBehaviorConfig
{
	GENERATED_BODY()
public:
	FInteractBehaviorConfig()
	{
		Behaviors.Add(FInteractBehavior());
	}

	UPROPERTY()
	ACharacterBase* User;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "行为")
	TArray<FInteractBehavior> Behaviors;
};

UCLASS(abstract)
class ARPG_API UARPG_InteractableActorManagerBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UARPG_InteractableActorManagerBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void StartInteract(ACharacterBase* Invoker, const FOnInteractFinished& OnInteractFinished);

	void EndInteract(ACharacterBase* Invoker, const FOnInteractAbortFinished& OnInteractAbortFinished);
private:
	void StartInteractImpl(ACharacterBase* Invoker, const FInteractBehavior* InvokeBehavior, const FInteractBehaviorConfig* InvokeConfig, const FOnInteractFinished &OnInteractFinished);

	void WhenInteractFinishedSucceed(const FName& Tag, ACharacterBase* Invoker, FOnInteractFinished OnInteractFinished);

	void WhenInteractFinishedFailed(ACharacterBase* Invoker, FOnInteractFinished OnInteractFinished);

	void WhenActionAbortFinished(ACharacterBase* Invoker, FOnInteractAbortFinished OnInteractAbortFinished);

	void InteractActorBeginSetCollision(ACharacterBase* Invoker);
	void InteractActorEndSetCollision(ACharacterBase* Invoker);

	void ExecuteWhenBeginInteract(ACharacterBase* Invoker, FInteractBehaviorConfig& InvokeConfig);
	void ExecuteWhenEndInteract(ACharacterBase* Invoker, bool bFinishPerfectly);
public:
	virtual void GetBehavior(ACharacterBase* Invoker, const FInteractBehaviorConfig*& ValidConfig, const FInteractBehavior*& ValidBehavior) const { }
	virtual bool CanInteract(const ACharacterBase* Invoker) const { return false; }
	virtual void PostMoveFinished(ACharacterBase* Invoker) {}
public:
	virtual void WhenBeginInteract(ACharacterBase* Invoker, const FInteractBehaviorConfig& InvokeConfig) {}
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInteractBegin, AActor*, Which, class UARPG_InteractableActorManagerBase*, Manager, class ACharacterBase*, Who);
	UPROPERTY(BlueprintAssignable, Category = "交互")
	FOnInteractBegin OnInteractBegin;

	virtual void WhenEndInteract(ACharacterBase* Invoker) {}
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnInteractEnd, AActor*, Which, class UARPG_InteractableActorManagerBase*, Manager, class ACharacterBase*, Who, bool, bFinishPerfectly);
	UPROPERTY(BlueprintAssignable, Category = "交互")
	FOnInteractEnd OnInteractEnd;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReceiveInteractEvent, class UARPG_InteractableActorManagerBase*, Manager, class ACharacterBase*, Who, const FGameplayTag&, EventTag);
	UPROPERTY(BlueprintAssignable, Category = "交互")
	FOnReceiveInteractEvent OnReceiveInteractEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReceiveInteractStateEventBegin, class UARPG_InteractableActorManagerBase*, Manager, class ACharacterBase*, Who, const FGameplayTag&, EventTag);
	UPROPERTY(BlueprintAssignable, Category = "交互")
	FOnReceiveInteractStateEventBegin OnReceiveInteractStateEventBegin;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnReceiveInteractStateEventEnd, class UARPG_InteractableActorManagerBase*, Manager, class ACharacterBase*, Who, const FGameplayTag&, EventTag, bool, bFinishPerfectly);
	UPROPERTY(BlueprintAssignable, Category = "交互")
	FOnReceiveInteractStateEventEnd OnReceiveInteractStateEventEnd;

	UPROPERTY(EditAnywhere, Category = "交互")
	uint8 bCancelCapsuleCollision : 1;

	UPROPERTY(EditAnywhere, Category = "交互")
	uint8 bForceEnterReleaseState : 1;
private:
	UPROPERTY()
	TMap<ACharacterBase*, UARPG_AD_InteractableBase*> CurBehaviorMap;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, DisplayName = "交互管理器_单人使用"))
class ARPG_API UInteractableActorManagerSingle : public UARPG_InteractableActorManagerBase
{
	GENERATED_BODY()
public:
	UInteractableActorManagerSingle();

	bool CanInteract(const ACharacterBase* Invoker) const override { return Config.User == nullptr; }

	void GetBehavior(ACharacterBase* Invoker, const FInteractBehaviorConfig*& ValidConfig, const FInteractBehavior*& ValidBehavior) const override;

	void WhenEndInteract(ACharacterBase* Invoker) override;
public:
	const TArray<FInteractBehavior>& GetBehaviors() const { return Config.Behaviors; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "配置", meta = (ShowOnlyInnerProperties = true))
	FInteractBehaviorConfig Config;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, DisplayName = "交互管理器_多人使用"))
class ARPG_API UInteractableActorManagerMulti : public UARPG_InteractableActorManagerBase
{
	GENERATED_BODY()
public:
	UInteractableActorManagerMulti();

	bool CanInteract(const ACharacterBase* Invoker) const override;

	void GetBehavior(ACharacterBase* Invoker, const FInteractBehaviorConfig*& ValidConfig, const FInteractBehavior*& ValidBehavior) const override;

	void WhenEndInteract(ACharacterBase* Invoker) override;
public:
	UPROPERTY(EditAnywhere, Category = "配置")
	TArray<FInteractBehaviorConfig> Configs;
};
