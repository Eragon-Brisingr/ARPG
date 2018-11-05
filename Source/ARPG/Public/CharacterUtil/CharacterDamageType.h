// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <SubclassOf.h>
#include "CharacterDamageType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPhysicsDamageType : uint8
{
	Hit UMETA(DisplayName = "打击"),
	Slash UMETA(DisplayName = "斩击"),
	Stab UMETA(DisplayName = "突刺")
};

USTRUCT(BlueprintType)
struct ARPG_API FApplyPointDamageParameter
{
	GENERATED_BODY()
public:
	FApplyPointDamageParameter(float AddHitStunValue = 10.f, const TSubclassOf<class UReceiveDamageActionBase>& ReceiveDamageAction = nullptr, float NormalBeakBackDistance = 20.f, float DefenseBeakBackDistance = 20.f, bool bCanDefense = true, bool bCanDefenseSwipe = true, bool bCanDodge = true)
		:AddHitStunValue(AddHitStunValue), ReceiveDamageAction(ReceiveDamageAction), NormalBeakBackDistance(NormalBeakBackDistance), DefenseBeakBackDistance(DefenseBeakBackDistance), bCanDefense(bCanDefense), bCanDefenseSwipe(bCanDefenseSwipe), bCanDodge(bCanDodge)
	{}

	UPROPERTY(EditAnywhere, Category = "伤害")
	uint8 bCanDodge : 1;
	UPROPERTY(EditAnywhere, Category = "伤害")
	uint8 bCanDefense : 1;
	UPROPERTY(EditAnywhere, Category = "伤害")
	uint8 bCanDefenseSwipe : 1;

	UPROPERTY(EditAnywhere, Category = "伤害")
	float AddHitStunValue;
	UPROPERTY(EditAnywhere, Category = "伤害")
	TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction;

	//只在不播放任何受击动画时的推动距离
	UPROPERTY(EditAnywhere, Category = "伤害")
	float NormalBeakBackDistance;
	//防御时受击时的推动距离
	UPROPERTY(EditAnywhere, Category = "伤害")
	float DefenseBeakBackDistance;
};
