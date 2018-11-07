// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ARPG_HUDBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_HUDBase : public AHUD
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	AARPG_HUDBase();

	UPROPERTY(VisibleAnywhere, Category = "交互提示系统")
	class USphereComponent* HintInfoCollector;
	UFUNCTION()
	void WhenHintInfoCollectorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WhenHintInfoCollectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY()
	TArray<AActor*> PotentialInteractableActors;

	UFUNCTION(BlueprintCallable, Category = "交互提示系统")
	AActor* GetNearestInteractableActor();
};
