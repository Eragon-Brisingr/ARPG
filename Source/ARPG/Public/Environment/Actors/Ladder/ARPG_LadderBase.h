// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARPG_InteractInterface.h"
#include "ARPG_LadderBase.generated.h"

class ACharacterBase;
class UAnimMontage;
class UNavLinkCustomComponent;
class UARPG_ReceiveDamageActionBase;

USTRUCT(BlueprintType, BlueprintInternalUseOnly)
struct FInLadderData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "梯子")
	int32 LadderIndex;
};

USTRUCT()
struct FClimbLadderConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "梯子")
	int32 DownEnterSlot = 2;
	UPROPERTY(EditAnywhere, Category = "梯子")
	UAnimMontage* DownEnterMontage;
	UPROPERTY(EditAnywhere, Category = "梯子")
	int32 DownLeaveSlot = 2;
	UPROPERTY(EditAnywhere, Category = "梯子")
	UAnimMontage* DownLeaveMontage;

	UPROPERTY(EditAnywhere, Category = "梯子")
	int32 UpEnterSlot = 6;
	UPROPERTY(EditAnywhere, Category = "梯子")
	UAnimMontage* UpEnterMontage;
	UPROPERTY(EditAnywhere, Category = "梯子")
	int32 UpLeaveSlot = 6;
	UPROPERTY(EditAnywhere, Category = "梯子")
	UAnimMontage* UpLeaveMontage;

	UPROPERTY(EditAnywhere, Category = "梯子")
	int32 ClimbUpSlotNumber = 1;
	UPROPERTY(EditAnywhere, Category = "梯子")
	UAnimMontage* UpLoopMontage;
	UPROPERTY(EditAnywhere, Category = "梯子")
	int32 ClimbDownSlotNumber = 1;
	UPROPERTY(EditAnywhere, Category = "梯子")
	UAnimMontage* DownLoopMontage;

	UPROPERTY(EditAnywhere, Category = "梯子")
	UAnimMontage* IdleLoopMontage;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = "梯子")
	FTransform DownEnterLocation;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = "梯子")
	FTransform UpEnterLocation;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = "梯子")
	FVector ClimbingLocation;

	UPROPERTY(EditAnywhere, Category = "梯子")
	UARPG_ReceiveDamageActionBase* LadderReceiveDamageAction;
};

UCLASS()
class ARPG_API AARPG_LadderBase : public AActor, public IARPG_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARPG_LadderBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

	void WhenInvokeInteract_Implementation(ACharacterBase* InteractInvoker) override;

	bool CanInteract_Implementation(const ACharacterBase* InteractInvoker) const override;
	void WhenBeginInteract_Implementation(ACharacterBase* InteractInvoker) override;
	void WhenEndInteract_Implementation(ACharacterBase* InteractInvoker) override;
protected:
	int32 GetCharacterHeightUnit(const ACharacterBase* Character) const;
	bool CanCharacterInSlot(const ACharacterBase* Character, int32 Slot) const;
	FVector GetSlotWorldLocation(const ACharacterBase* Character, int32 Slot) const;

	void PlayLadderMontage(ACharacterBase* Character, UAnimMontage* Montage, int32 NextSlot);

	void WhenEnterLadder(ACharacterBase* Character);
	void WhenLeaveLadder(ACharacterBase* Character);

	void EnterLadderImpl(ACharacterBase* Character, bool IsLowEnter);

public:
	bool CanDownEnter(const ACharacterBase* Character) const;
	bool CanUpEnter(const ACharacterBase* Character) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "梯子")
	int32 LadderLength = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "梯子")
	float LadderUnitHeight = 25.f;

	UPROPERTY(BlueprintReadOnly, Category = "梯子")
	TMap<ACharacterBase*, FInLadderData> CharacterInLadderDatas;

	UPROPERTY(EditAnywhere, Category = "梯子")
	TMap<TSubclassOf<ACharacterBase>, FClimbLadderConfig> ClimbLadderConfigs;

	const FClimbLadderConfig* GetClimbConfig(const ACharacterBase* Character) const;

public:
	UPROPERTY(VisibleAnywhere, Category = "梯子")
	UNavLinkCustomComponent* LadderNavLinkCustomComponent;

	void NotifySmartLinkReached(UNavLinkCustomComponent* LinkComp, UObject* PathingAgent, const FVector& DestPoint);
};
