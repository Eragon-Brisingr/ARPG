// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XD_SaveGameInterface.h"
#include "CharacterBase.generated.h"

UCLASS()
class ARPG_API ACharacterBase : public ACharacter, public IXD_SaveGameInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& PCIP);

	UPROPERTY(BlueprintReadOnly, Category = "角色|移动")
	class UARPG_MovementComponent* ARPG_MovementComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//播放Montage
public:
	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	float PlayMontage(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None, bool ClientMaster = false);

	UFUNCTION(Reliable, WithValidation, NetMulticast)
	void MulticastPlayMontage(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	void MulticastPlayMontage_Implementation(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	bool MulticastPlayMontage_Validate(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None) { return true; }

	UFUNCTION(Reliable, WithValidation, NetMulticast)
	void MulticastPlayMontageSkipOwner(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	void MulticastPlayMontageSkipOwner_Implementation(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	bool MulticastPlayMontageSkipOwner_Validate(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None) { return true; }

	UFUNCTION(BlueprintCallable, Category = "角色|行为", Reliable, WithValidation, NetMulticast)
	void StopMontage();
	virtual void StopMontage_Implementation();
	bool StopMontage_Validate() { return true; }

	UFUNCTION(Reliable, WithValidation, Server)
	void PlayMontageToServer(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	virtual void PlayMontageToServer_Implementation(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	bool PlayMontageToServer_Validate(UAnimMontage * MontageToPlay, float InPlayRate = 1.f, FName StartSectionName = NAME_None) { return true; }
	
};
