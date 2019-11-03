// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterAnimType.h"
#include "ARPG_MontagePlayCondition_Impl.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "下落攻击_落地跳转"))
class ARPG_API UAPC_FallingAttack_OnLand : public UARPG_AnimPlayCondition
{
	GENERATED_BODY()
public:
	virtual bool CanPlayMontage_Implementation(class ACharacterBase* Character) const override;
};

UCLASS(meta = (DisplayName = "角色_存活"))
class ARPG_API UAPC_Character_Alive : public UARPG_AnimPlayCondition
{
	GENERATED_BODY()
public:
	virtual bool CanPlayMontage_Implementation(class ACharacterBase* Character) const override;
};
