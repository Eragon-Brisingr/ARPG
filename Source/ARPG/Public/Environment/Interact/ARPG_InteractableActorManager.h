// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "ARPG_InteractType.h"
#include "ARPG_CharacterBehaviorType.h"
#include "ARPG_InteractableActorManager.generated.h"

class UARPG_CharacterBehaviorConfigBase;
class ACharacterBase;

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

	void WhenMoveFinished(const FPathFollowingResult& Result, ACharacterBase* Invoker, FVector Location, FRotator Rotation, FOnInteractFinished OnInteractFinished);

	void EndInteract(ACharacterBase* Invoker, const FOnInteractAbortFinished& OnInteractAbortFinished);

	void WhenBehaviorAbortFinished(ACharacterBase* Invoker, FOnInteractAbortFinished OnInteractAbortFinished);

public:
	virtual FBehaviorWithPosition GetBehavior(ACharacterBase* Invoker, const FVector& InteractableLocation) const { return {}; }
	virtual bool CanInteract(const ACharacterBase* Invoker) const { return false; }
	virtual void PostMoveFinished(ACharacterBase* Invoker) {}
public:
	virtual void WhenBeginInteract(ACharacterBase* Invoker) {}
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBeginInteract, AActor*, Which, class UARPG_InteractableActorManagerBase*, Manager, class ACharacterBase*, Who);
	UPROPERTY(BlueprintAssignable, Category = "交互")
	FOnBeginInteract OnBeginInteract;

	virtual void WhenEndInteract(ACharacterBase* Invoker) {}
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEndInteract, AActor*, Which, class UARPG_InteractableActorManagerBase*, Manager, class ACharacterBase*, Who);
	UPROPERTY(BlueprintAssignable, Category = "交互")
	FOnEndInteract OnEndInteract;
private:
	UPROPERTY()
	TMap<ACharacterBase*, UARPG_CharacterBehaviorConfigBase*> CurBehaviorMap;

	virtual void GetInteractableLocationAndRotation(ACharacterBase* Invoker, FVector& InteractableLocation, FRotator& InteractableRotation) const;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, DisplayName = "交互管理器_单人使用"))
class ARPG_API UInteractableActorManagerSingle : public UARPG_InteractableActorManagerBase
{
	GENERATED_BODY()
public:
	UInteractableActorManagerSingle();

	virtual bool CanInteract(const ACharacterBase* Invoker) const { return User == nullptr; }

	virtual FBehaviorWithPosition GetBehavior(ACharacterBase* Invoker, const FVector& InteractableLocation) const;

	virtual void WhenBeginInteract(ACharacterBase* Invoker) { User = Invoker; }

	virtual void WhenEndInteract(ACharacterBase* Invoker) { User = nullptr; }

	virtual void GetInteractableLocationAndRotation(ACharacterBase* Invoker, FVector& InteractableLocation, FRotator& InteractableRotation) const;
public:
	UPROPERTY()
	ACharacterBase* User;

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<FBehaviorWithPosition> Behaviors;
};
