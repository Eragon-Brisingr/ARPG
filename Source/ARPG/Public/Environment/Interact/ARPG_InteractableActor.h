// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARPG_InteractInterface.h"
#include "ARPG_InteractType.h"
#include "ARPG_InteractableActor.generated.h"

class ACharacterBase;
class UARPG_InteractableActorManagerBase;

UCLASS()
class ARPG_API AARPG_InteractableActorBase : public AActor,
	public IARPG_InteractInterface
{
	GENERATED_BODY()
public:
	AARPG_InteractableActorBase();

	UPROPERTY(VisibleAnywhere, Category = "行为")
	UARPG_InteractableActorManagerBase* InteractableActorManager;

	void StartInteract(ACharacterBase* Invoker, const FOnInteractFinished& OnInteractFinished);

	void EndInteract(ACharacterBase* Invoker, const FOnInteractAbortFinished& OnInteractAbortFinished);
public:
	virtual bool CanInteract_Implementation(const class ACharacterBase* InteractInvoker) const;

	virtual void WhenInvokeInteract_Implementation(class ACharacterBase* InteractInvoker);
};
