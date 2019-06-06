// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_LightDetector.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "ARPG_AI_Config.h"
#include "XD_TextureFunctionLibrary.h"
#include "TextureResource.h"
#include "GameFramework/GameStateBase.h"

// Sets default values for this component's properties
AARPG_LightDetector::AARPG_LightDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = false;

	bHidden = true;

	LightDetectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(GET_MEMBER_NAME_CHECKED(AARPG_LightDetector, LightDetectorMesh));
	{
		LightDetectorMesh->SetStaticMesh(GetDefault<UARPG_AI_Config>()->LightDetectorMesh.LoadSynchronous());
	}
	SetRootComponent(LightDetectorMesh);

	TopLightRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(GET_MEMBER_NAME_CHECKED(AARPG_LightDetector, TopLightRenderTarget));
	{
		TopLightRenderTarget->LODGroup = TextureGroup::TEXTUREGROUP_Pixels2D;
		TopLightRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGB10A2;
		TopLightRenderTarget->InitAutoFormat(4, 4);
		TopLightRenderTarget->UpdateResourceImmediate(true);
	}

	TopLightCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(GET_MEMBER_NAME_CHECKED(AARPG_LightDetector, TopLightCapture));
	{
		TopLightCapture->SetupAttachment(LightDetectorMesh);
		TopLightCapture->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 65.f), FRotator(-90.f, 0.f, 45.f));
		TopLightCapture->ProjectionType = ECameraProjectionMode::Perspective;
		TopLightCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDRNoAlpha;
		TopLightCapture->FOVAngle = 50.f;
		TopLightCapture->TextureTarget = TopLightRenderTarget;
		TopLightCapture->bCaptureEveryFrame = false;
		TopLightCapture->bCaptureOnMovement = false;
		TopLightCapture->ShowFlags.SetParticles(false);
		TopLightCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	}
}


// Called when the game starts
void AARPG_LightDetector::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void AARPG_LightDetector::Destroyed()
{
	Super::Destroyed();
}

float AARPG_LightDetector::SampleLightBrightness()
{
	QUICK_SCOPE_CYCLE_COUNTER(AARPG_LightDetector_SampleLightBrightness);
	SetActorHiddenInGame(false);
	TopLightCapture->CaptureScene();
	SetActorHiddenInGame(true);
	TArray<FColor> Pixels;
	FTextureRenderTargetResource* RenderTarget = TopLightRenderTarget->GameThread_GetRenderTargetResource();
	RenderTarget->ReadPixels(Pixels);
	return UXD_TextureFunctionLibrary::GetPixelsMaxBrightness(Pixels) / 255.f;
}

float AARPG_LightDetector::SampleActorBrightness(AActor* Actor)
{
	check(Actor);
	float CurGameTime = GetWorld()->GetTimeSeconds();
	FLightDetectData& LightDetectData = ActorLightDatas.FindOrAdd(Actor);
	if (CurGameTime - LightDetectData.PreSampleTime > SampleInterval)
	{
		SetActorScale3D(Actor->GetActorScale3D());
		SetActorLocation(Actor->GetActorLocation());
		Actor->GetAttachedActors(TopLightCapture->HiddenActors);
		TopLightCapture->HiddenActors.Add(Actor);
		LightDetectData.PreSampleTime = CurGameTime;
		LightDetectData.Brightness = SampleLightBrightness();
	}
	return LightDetectData.Brightness;
}

float AARPG_LightDetector::SampleLocalBrightness(const FVector& Location)
{
	SetActorScale3D(FVector::OneVector);
	SetActorLocation(Location);
	return SampleLightBrightness();
}

TWeakObjectPtr<AARPG_LightDetector> UARPG_LightDetectLibrary::CachedLightDetector;

AARPG_LightDetector* UARPG_LightDetectLibrary::GetLightDetector(const UObject* WorldContextObject)
{
	if (CachedLightDetector.IsValid())
	{
		return CachedLightDetector.Get();
	}

	if (AGameStateBase* GameState = WorldContextObject->GetWorld()->GetGameState())
	{
		TArray<AActor*> AttachedActors;
		GameState->GetAttachedActors(AttachedActors);
		for (AActor* Actor : AttachedActors)
		{
			if (AARPG_LightDetector* LightDetector = Cast<AARPG_LightDetector>(Actor))
			{
				CachedLightDetector = LightDetector;
				return LightDetector;
			}
		}

		AARPG_LightDetector* LightDetector = GameState->GetWorld()->SpawnActor<AARPG_LightDetector>();
		CachedLightDetector = LightDetector;
		LightDetector->AttachToActor(GameState, FAttachmentTransformRules::SnapToTargetIncludingScale);
		return LightDetector;
	}
	return nullptr;
}

float UARPG_LightDetectLibrary::SampleActorBrightness(AActor* Actor)
{
	if (Actor)
	{
		return GetLightDetector(Actor)->SampleActorBrightness(Actor);
	}
	return 0.f;
}

float UARPG_LightDetectLibrary::SampleLocalBrightness(const UObject* WorldContextObject, const FVector& Location)
{
	return GetLightDetector(WorldContextObject)->SampleLocalBrightness(Location);
}
