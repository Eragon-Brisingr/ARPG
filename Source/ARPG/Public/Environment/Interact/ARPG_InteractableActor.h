// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARPG_InteractInterface.h"
#include "XD_DrawGizmoInterface.h"
#include "ARPG_InteractableActor.generated.h"

class ACharacterBase;
class UARPG_AD_InteractableBase;

USTRUCT(BlueprintType)
struct FARPG_InteractSingleConfig
{
	GENERATED_BODY()
public:
	FARPG_InteractSingleConfig()
		:bSnapRotation(true)
	{}

	UPROPERTY(EditAnywhere, Category = "行为")
	float StartBehaviousRadius;

	UPROPERTY(EditAnywhere, Category = "行为")
	uint8 bSnapRotation : 1;

	UPROPERTY(EditAnywhere, Category = "行为", meta = (MakeEditWidget = true))
	FTransform InteractLocation;

	UPROPERTY(EditAnywhere, Category = "行为", Instanced)
	UARPG_AD_InteractableBase* InteractDispatcher;

	void InitConfig(AActor* Owner);
	bool CanInteract(const ACharacterBase* InteractInvoker) const;
	void WhenInvokeInteract(AActor* Owner, ACharacterBase* InteractInvoker);
	void WhenAbortInteract(AActor* Owner, ACharacterBase* InteractInvoker);
	ACharacterBase* GetInteracter() const;
};

UCLASS(meta = (DisplayName = "单人交互Actor"), Blueprintable)
class ARPG_API AARPG_InteractableActorSingle : public AActor, 
	public IARPG_InteractInterface,
	public IXD_DrawGizmoInterface
{
	GENERATED_BODY()
public:
	AARPG_InteractableActorSingle();
	
	UPROPERTY(EditAnywhere, Category = "行为", meta = (ShowOnlyInnerProperties))
	FARPG_InteractSingleConfig InteractSingleConfig;

	UFUNCTION()
	ACharacterBase* GetInteracter() const;

	bool CanInteract_Implementation(const ACharacterBase* InteractInvoker) const override;
	void WhenInvokeInteract_Implementation(ACharacterBase* InteractInvoker) override;
	void WhenAbortInteract_Implementation(ACharacterBase* InteractInvoker) override;

public:
	void BeginPlay() override;

#if WITH_EDITOR
	void DrawGizmoNative(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI, bool IsSelected) override;
#endif
};
