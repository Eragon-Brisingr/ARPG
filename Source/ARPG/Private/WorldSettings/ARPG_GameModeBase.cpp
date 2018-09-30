// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_GameModeBase.h"
#include "XD_SaveGameFunctionLibrary.h"




void AARPG_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UXD_SaveGameFunctionLibrary::InitAutoSaveLoadSystem(this);
}

void AARPG_GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UXD_SaveGameFunctionLibrary::ShutDownAutoSaveLoadSystem(this);
}

APawn* AARPG_GameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(NewPlayer))
	{
		if (APawn* Pawn = UXD_SaveGameFunctionLibrary::TryLoadPlayer(PlayerController))
		{
			UXD_SaveGameFunctionLibrary::RegisterAutoSavePlayer(Pawn);
			return Pawn;
		}
	}
	APawn* Pawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
	UXD_SaveGameFunctionLibrary::RegisterAutoSavePlayer(Pawn);
	return Pawn;
}

APawn* AARPG_GameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(NewPlayer))
	{
		if (APawn* Pawn = UXD_SaveGameFunctionLibrary::TryLoadPlayer(PlayerController))
		{
			UXD_SaveGameFunctionLibrary::RegisterAutoSavePlayer(Pawn);
			return Pawn;
		}
	}
	APawn* Pawn = Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
	UXD_SaveGameFunctionLibrary::RegisterAutoSavePlayer(Pawn);
	return Pawn;
}
