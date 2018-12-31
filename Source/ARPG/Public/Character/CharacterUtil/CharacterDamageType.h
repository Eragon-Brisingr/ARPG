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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "伤害", meta = (DisplayName = "能闪避该伤害"))
	uint8 bCanDodge : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "伤害", meta = (DisplayName = "能防御该伤害"))
	uint8 bCanDefense : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "伤害", meta = (DisplayName = "能防反该伤害"))
	uint8 bCanDefenseSwipe : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "伤害", meta = (DisplayName = "增加削韧量"))
	float AddHitStunValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "伤害", meta = (DisplayName = "受击特殊动作"))
	TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction;

	//只在不播放任何受击动画时的推动距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "伤害", meta = (DisplayName = "击退距离"))
	float NormalBeakBackDistance;
	//防御时受击时的推动距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "伤害", meta = (DisplayName = "防御时击退距离"))
	float DefenseBeakBackDistance;
};
