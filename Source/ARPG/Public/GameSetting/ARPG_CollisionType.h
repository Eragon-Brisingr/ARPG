// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine/EngineTypes.h>
// #include "UObject/NoExportTypes.h"
// #include "ARPG_CollisionType.generated.h"


/**
 * 
 */
struct FARPG_CollisionObjectType
{
	static const EObjectTypeQuery Pawn;
	static constexpr EObjectTypeQuery CharacterMesh = EObjectTypeQuery::ObjectTypeQuery7;
	static constexpr EObjectTypeQuery Item = EObjectTypeQuery::ObjectTypeQuery8;
};

struct FARPG_TraceTypeQuery
{
	static const ETraceTypeQuery Visibility;
	static const ETraceTypeQuery Camera;
	static const ETraceTypeQuery AI_Sight;
};

struct FARPG_CollisionProfile
{
	static const FName Item;
	static const FName ArrowReleasing;
};
