// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARPG_InteractInterface.h"
#include "ARPG_InteractType.h"
#include "ARPG_InteractableActor.generated.h"

class ACharacterBase;
class UARPG_InteractableActorManagerBase;
class UInteractableActorManagerSingle;
class UInteractableActorManagerMulti;

UCLASS(abstract, NotBlueprintable)
class ARPG_API AARPG_InteractableActorBase : public AActor,
	public IARPG_InteractInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "行为")
	UARPG_InteractableActorManagerBase* InteractableActorManager;

	void StartInteract(ACharacterBase* Invoker, const FOnInteractFinished& OnInteractFinished);

	void EndInteract(ACharacterBase* Invoker, const FOnInteractAbortFinished& OnInteractAbortFinished);
public:
	virtual bool CanInteract_Implementation(const class ACharacterBase* InteractInvoker) const;

	virtual void WhenInvokeInteract_Implementation(class ACharacterBase* InteractInvoker);
};

UCLASS(meta = (DisplayName = "单人交互Actor"), Blueprintable)
class ARPG_API AARPG_InteractableActorSingle : public AARPG_InteractableActorBase
{
	GENERATED_BODY()
public:
	AARPG_InteractableActorSingle();

	UPROPERTY(BlueprintReadOnly, Category = "行为")
	UInteractableActorManagerSingle* InteractableActorManagerSingle;
};

UCLASS(meta = (DisplayName = "多人交互Actor"), Blueprintable)
class ARPG_API AARPG_InteractableActorMulti : public AARPG_InteractableActorBase
{
	GENERATED_BODY()
public:
	AARPG_InteractableActorMulti();

	UPROPERTY(BlueprintReadOnly, Category = "行为")
	UInteractableActorManagerMulti* InteractableActorManagerMulti;
};

