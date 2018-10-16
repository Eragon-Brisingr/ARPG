// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CollisionType.h"




const EObjectTypeQuery FARPG_CollisionObjectType::Pawn = UEngineTypes::ConvertToObjectType(ECC_Pawn);

const ETraceTypeQuery FARPG_TraceTypeQuery::Visibility = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);

const ETraceTypeQuery FARPG_TraceTypeQuery::Camera = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera);

const FName FARPG_CollisionProfile::Item = TEXT("Item");

const FName FARPG_CollisionProfile::ArrowReleasing = TEXT("ArrowReleasing");

