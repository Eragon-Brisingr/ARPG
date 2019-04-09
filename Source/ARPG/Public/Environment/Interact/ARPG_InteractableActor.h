// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARPG_InteractInterface.h"
#include "ARPG_InteractableActor.generated.h"

class ACharacterBase;
class UARPG_AD_InteractableBase;

UCLASS(meta = (DisplayName = "单人交互Actor"), Blueprintable)
class ARPG_API AARPG_InteractableActorSingle : public AActor, 
	public IARPG_InteractInterface
{
	GENERATED_BODY()
public:
	AARPG_InteractableActorSingle();

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FTransform InteractLocation;

	UPROPERTY(EditAnywhere, Category = "行为", Instanced)
	UARPG_AD_InteractableBase* InteractDispatcher;

	UFUNCTION()
	ACharacterBase* GetInteracter() const;

	bool CanInteract_Implementation(const ACharacterBase* InteractInvoker) const override;
	void WhenInvokeInteract_Implementation(ACharacterBase* InteractInvoker) override;
	void WhenAbortInteract_Implementation(ACharacterBase* InteractInvoker) override;

public:
	void BeginPlay() override;
};
