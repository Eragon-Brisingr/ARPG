// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ARPG_LightDetector.generated.h"

class UStaticMeshComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

USTRUCT()
struct FLightDetectData
{
	GENERATED_BODY()
public:
	float PreSampleTime = TNumericLimits<float>::Lowest();
	float Brightness;
};

UCLASS(Transient)
class ARPG_API AARPG_LightDetector : public AActor
{
	GENERATED_BODY()

public:	
	AARPG_LightDetector();

protected:
	void BeginPlay() override;
	void Destroyed() override;
public:
	UPROPERTY()
	UStaticMeshComponent* LightDetectorMesh;
	UPROPERTY(VisibleAnywhere, Transient)
	USceneCaptureComponent2D* TopLightCapture;
	UPROPERTY(Transient, VisibleAnywhere, Category = "Settings")
	UTextureRenderTarget2D* TopLightRenderTarget;

	float SampleLightBrightness();

	float SampleInterval = 0.2f;

	UPROPERTY(Transient)
	TMap<AActor*, FLightDetectData> ActorLightDatas;

	float SampleActorBrightness(AActor* Actor);

	float SampleLocalBrightness(const FVector& Location);
};

UCLASS()
class ARPG_API UARPG_LightDetectLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static TWeakObjectPtr<AARPG_LightDetector> CachedLightDetector;
	static AARPG_LightDetector* GetLightDetector(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AI|¸Ð¹Ù")
	static float SampleActorBrightness(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "AI|¸Ð¹Ù", meta = (WorldContext = WorldContextObject))
	static float SampleLocalBrightness(const UObject* WorldContextObject, const FVector& Location);
};
