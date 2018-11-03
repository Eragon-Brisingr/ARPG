// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine/EngineTypes.h>
// #include "UObject/NoExportTypes.h"
// #include "ARPG_CollisionType.generated.h"


/**
 * 
 */
namespace FARPG_CollisionObjectType
{
	static constexpr EObjectTypeQuery Pawn = EObjectTypeQuery::ObjectTypeQuery3;
	static constexpr EObjectTypeQuery CharacterMesh = EObjectTypeQuery::ObjectTypeQuery7;
	static constexpr EObjectTypeQuery Item = EObjectTypeQuery::ObjectTypeQuery8;
};

namespace FARPG_ECollisionChannel
{
	static constexpr ECollisionChannel AI_Sight = ECollisionChannel::ECC_GameTraceChannel3;
};

namespace FARPG_TraceQueryType
{
	static constexpr ETraceTypeQuery Visibility = ETraceTypeQuery::TraceTypeQuery1;
	static constexpr ETraceTypeQuery Camera = ETraceTypeQuery::TraceTypeQuery2;
	static constexpr ETraceTypeQuery AI_Sight = ETraceTypeQuery::TraceTypeQuery3;
};

struct FARPG_CollisionProfile
{
	static const FName Item;
	static const FName ArrowReleasing;
};
