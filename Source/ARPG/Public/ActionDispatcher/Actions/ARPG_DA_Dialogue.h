// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/XD_DispatchableActionBase.h"
#include "ARPG_DA_Dialogue.generated.h"

class ACharacterBase;
class USoundBase;

/**
 * 
 */
UCLASS(meta = (DisplayName = "说一句话"))
class ARPG_API UARPG_DA_SpeakOneSentence : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	TSet<AActor*> GetAllRegistableEntities() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
public:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "说话者"))
	TSoftObjectPtr<ACharacterBase> Speaker;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "说的话"))
	USoundBase* Sentence;

	UPROPERTY(SaveGame, BlueprintReadWrite, meta = (DisplayName = "当说完了"))
	FOnDispatchableActionFinishedEvent WhenSpeakFinished;
};
