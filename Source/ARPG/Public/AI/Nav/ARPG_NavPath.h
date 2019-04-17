// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "XD_DrawGizmoInterface.h"
#include "ARPG_CharacterBehaviorType.h"
#include "ARPG_NavPath.generated.h"

class UARPG_AD_CharacterBase;
class ACharacterBase;

USTRUCT(BlueprintType)
struct ARPG_API FARPG_NavPathPoint : public FBehaviorPositionData
{
	GENERATED_BODY()
public:
	FARPG_NavPathPoint()
	{
		bAttachToRotation = true;
	}

	UPROPERTY(EditAnywhere, Category = "行为")
	float AcceptableRadius = 100.f;
	float GetAcceptableRadius() const;
	
	//TODO 改为动作调度器，并且为多个调用
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "行为", Instanced)
	UARPG_AD_CharacterBase* BehaviorTemplate = nullptr;

	UPROPERTY()
	TMap<ACharacterBase*, UARPG_AD_CharacterBase*> BehaviorMap;

	UARPG_AD_CharacterBase* GetBehavior(ACharacterBase* Invoker);
};

UCLASS()
class ARPG_API UARPG_NavPathVisualHelper : public USplineComponent
{
	GENERATED_BODY()
public:

};

UCLASS(meta = (DisplayName = "路径"))
class ARPG_API AARPG_NavPath : public AActor, 
	public IXD_DrawGizmoInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARPG_NavPath();

	UPROPERTY(EditAnywhere, Category = "路径")
	uint8 bIsClosedLoop : 1;

	UPROPERTY(EditAnywhere, Category = "路径")
	TArray<FARPG_NavPathPoint> NavPathPoints;

#if WITH_EDITORONLY_DATA
	class UARPG_NavPathVisualHelper* VisualControl;

	void OnConstruction(const FTransform& Transform) override;

	void DrawGizmoNative(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI, bool IsSelected) override;
#endif
};


