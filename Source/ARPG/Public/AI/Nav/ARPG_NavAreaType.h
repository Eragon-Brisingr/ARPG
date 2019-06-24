// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavArea.h"
#include "NavMeshPath.h"
#include "ARPG_NavAreaType.generated.h"

/**
 * 
 */
UENUM()
enum class EARPG_NavAreaFlag : uint8
{
	// up to 15 values
	Default,
	Jump
	// and what not
};

namespace FARPG_NavAreaFlagHelper
{
	FORCEINLINE bool IsSet(uint16 Flags, EARPG_NavAreaFlag Bit) { return (Flags & (1 << static_cast<uint8>(Bit))) != 0; }
	FORCEINLINE bool IsSet(const FNavPathPoint& PathVert, EARPG_NavAreaFlag Bit) { return IsSet(FNavMeshNodeFlags(PathVert.Flags).AreaFlags, Bit); }
	FORCEINLINE void Set(uint16& Flags, EARPG_NavAreaFlag Bit) { Flags |= (1 << static_cast<uint8>(Bit)); }
	FORCEINLINE bool IsNavLink(const FNavPathPoint& PathVert) { return (FNavMeshNodeFlags(PathVert.Flags).PathFlags & RECAST_STRAIGHTPATH_OFFMESH_CONNECTION) != 0; }
}

UCLASS(hidedropdown)
class ARPG_API UARPG_NavArea_JumpBase : public UNavArea
{
	GENERATED_BODY()
public:
	UARPG_NavArea_JumpBase();
};

UCLASS(hidedropdown)
class ARPG_API UARPG_NavArea_LadderBase : public UNavArea
{
	GENERATED_BODY()
public:
	UARPG_NavArea_LadderBase();
};
