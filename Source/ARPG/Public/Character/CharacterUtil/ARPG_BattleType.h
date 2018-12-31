// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_BattleType.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ARPG_API FARPG_AttackInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|行为")
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|行为")
	FVector Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|行为")
	float Radius;

	const bool operator==(const FARPG_AttackInfo& AttackInfo) const { return SocketName == AttackInfo.SocketName && Radius == AttackInfo.Radius && Offset == AttackInfo.Offset; }

	friend uint32 GetTypeHash(const FARPG_AttackInfo& AttackInfo) { return FCrc::MemCrc32(&AttackInfo, sizeof(FARPG_AttackInfo)); }
};
