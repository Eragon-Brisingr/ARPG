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
public:
	AARPG_HUDBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	//交互提示系统
public:
	UPROPERTY(VisibleAnywhere, Category = "交互提示系统")
	class USphereComponent* HintInfoCollector;
	UFUNCTION()
	void WhenHintInfoCollectorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WhenHintInfoCollectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	FVector OldCharacterLocation;
	UPROPERTY(EditAnywhere, Category = "交互提示系统")
	float InteractableActorsUpdateDistance = 50.f;

	UPROPERTY()
	TArray<AActor*> PotentialInteractableActors;
	UPROPERTY()
	TArray<AActor*> InteractableActors;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorEnableInteract, AActor*, Actor);
	UPROPERTY(BlueprintAssignable)
	FOnActorEnableInteract OnActorEnableInteract;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDisableInteract, AActor*, Actor);
	UPROPERTY(BlueprintAssignable)
	FOnActorDisableInteract OnActorDisableInteract;

	UFUNCTION(BlueprintCallable, Category = "交互提示系统")
	AActor* GetNearestInteractableActor() const;
};
