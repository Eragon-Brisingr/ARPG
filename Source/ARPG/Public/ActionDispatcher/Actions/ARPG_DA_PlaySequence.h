// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/XD_DA_PlaySequence.h"
#include "ARPG_DA_PlaySequence.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_DA_PlaySequence : public UXD_DA_PlaySequenceBase
{
	GENERATED_BODY()
public:
	bool MoveToSequencePlayLocation(APawn* Mover, const FVector& PlayLocation, const FRotator& PlayRotation, int32 MoverIdx) override;
	void WhenActionDeactived() override;

	void WhenMoveFinished(const FPathFollowingResult& Result, int32 MoverIdx, ACharacterBase* Character, FVector PlayLocation, FRotator PlayRotation);
};
