// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <GameFramework/Character.h>
#include "ExecuteActionSet.generated.h"

class UAnimMontage;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBackstabAnimData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画", meta = (DisplayName = "受击方动画"))
	UAnimMontage* BeAttackedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画", meta = (DisplayName = "攻击方动画"))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画", meta = (DisplayName = "播放动画时的相对位置", MakeEditWidget = "true"))
	FVector RelationLocation = {70.f, 0.f, 0.f};
};

USTRUCT(BlueprintType)
struct FExecuteAnimData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画", meta = (DisplayName = "受击方动画"))
	UAnimMontage* BeAttackedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画", meta = (DisplayName = "攻击方动画"))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画", meta = (DisplayName = "容差", MakeEditWidget = "true"))
	float Tolerance = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画", meta = (DisplayName = "播放动画时的相对位置", MakeEditWidget = "true"))
	FVector RelationLocation = {70.f, 0.f, 0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画", meta = (DisplayName = "偏转角偏转量"))
	float YawOffset;
};


USTRUCT(BlueprintType)
struct ARPG_API FExecuteActionSet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "背刺动画集"))
	TMap<TSubclassOf<class ACharacter>, FBackstabAnimData> BackstabMap;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "处决动画集"))
	TMap<UAnimMontage*, FExecuteAnimData> ExecuteMontageMap;

	//处决
	bool InvokeExecuteOther(class ACharacterBase* Invoker, class ACharacterBase* ExecuteTarget) const;

	bool TraceForExecuteOther(class ACharacterBase* Invoker);

	const FBackstabAnimData* FindBackstabAnimData(class ACharacterBase* Character) const;
};
