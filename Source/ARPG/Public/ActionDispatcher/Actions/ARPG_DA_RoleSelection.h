// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_DA_RoleSelectionBase.h"
#include "ARPG_DA_RoleSelection.generated.h"

class AActor;

/**
 * 
 */
UCLASS(abstract, meta = (DisplayName = "显示选项_交互"))
class ARPG_API UARPG_DA_RoleSelectionBase : public UXD_DA_RoleSelectionBase
{
	GENERATED_BODY()

};

UCLASS(meta = (DisplayName = "显示选项_交互"))
class ARPG_API UARPG_DA_RoleSelection_Interact : public UARPG_DA_RoleSelectionBase
{
	GENERATED_BODY()
public:
	UARPG_DA_RoleSelection_Interact();

	// 给主角交互选项的目标
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "交互方"))
	TSoftObjectPtr<AActor> InteractTarget;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "有效距离"))
	float ValidDistance = 300.f;

	void WhenActionActived() override;
	void WhenActionDeactived() override;
	bool IsActionValid() const override;
};
