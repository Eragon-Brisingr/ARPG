// Fill out your copyright notice in the Description page of Project Settings.

#include "SocketMoveTraceManager.h"
#include <Components/PrimitiveComponent.h>
#include "ARPG_CollisionType.h"
#include "DrawDebugHelpers.h"



FSocketMoveTracerConfig::FSocketMoveTracerConfig()
	:TraceTypeQuery(FARPG_TraceQueryType::Visibility)
{

}

USocketMoveTracer::USocketMoveTracer()
{

}

void USocketMoveTracer::Tick(float DeltaTime)
{
	DoTrace(DeltaTime);
}

UWorld* USocketMoveTracer::GetTickableGameObjectWorld() const
{
	return TargetSocketMesh.IsValid() ? TargetSocketMesh->GetWorld() : nullptr;
}

UWorld* USocketMoveTracer::GetWorld() const
{
	return TargetSocketMesh.IsValid() ? TargetSocketMesh->GetWorld() : nullptr;
}

AActor* USocketMoveTracer::GetOwner() const
{
	return TargetSocketMesh.IsValid() ? TargetSocketMesh->GetOwner() : nullptr;
}

void USocketMoveTracer::InitSocketMoveTracer(UPrimitiveComponent* TargetComponent, FSocketMoveTracerConfig& SocketMoveTracerConfig)
{
	Config = &SocketMoveTracerConfig;
	if (TargetComponent)
	{
		TargetSocketMesh = TargetComponent;
		TracedActors.Append({ GetOwner(), GetOwner()->GetOwner() });
		if (GetTraceSocketList().Num() < 2)
		{
			Config->TraceSocketList = TargetSocketMesh->GetAllSocketNames();
		}
		PrePosList.SetNum(GetTraceSocketList().Num());
	}
}

void USocketMoveTracer::EnableTrace(bool ClearIgnoreList /*= true*/)
{
	if (bEnableTrace != true && Config)
	{
		bEnableTrace = true;
		if (ClearIgnoreList)
		{
			TracedActors.RemoveAt(2, TracedActors.Num() - 2);
		}
		RecordPrePosition();
	}
}

void USocketMoveTracer::DisableTrace()
{
	if (bEnableTrace != false)
	{
		bEnableTrace = false;
	}
}

void USocketMoveTracer::DoTrace(float DeltaTime)
{
	if (!Config || !TargetSocketMesh.IsValid())
	{
		return;
	}

	TArray<AActor*> TracedInOnceTraceActor;
	for (int i = 1; i < GetTraceSocketList().Num(); ++i)
	{
		float StepAddAlpha = Config->StepLength / (PrePosList[i - 1] - PrePosList[i]).Size();

		FVector TraceStart_Start = PrePosList[i - 1];
		FVector TraceStart_End = PrePosList[i];
		FVector TraceEnd_Start = TargetSocketMesh->GetSocketLocation(GetTraceSocketList()[i - 1]);
		FVector TraceEnd_End = TargetSocketMesh->GetSocketLocation(GetTraceSocketList()[i]);

		for (float Alpha = 0; Alpha < 1.f; Alpha += StepAddAlpha)
		{
			FVector TraceStart = FMath::Lerp(TraceStart_Start, TraceStart_End, Alpha);
			FVector TraceEnd = FMath::Lerp(TraceEnd_Start, TraceEnd_End, Alpha);

			TArray<FHitResult> Hits;

#if WITH_EDITOR
			if (GetWorld()->IsGameWorld() == false)
			{
				struct FKismetTraceUtils
				{
					static void DrawDebugSphereTraceMulti(const UWorld* World, const FVector& Start, const FVector& End, float Radius, EDrawDebugTrace::Type DrawDebugType, bool bHit, const TArray<FHitResult>& OutHits, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
					{
						if (DrawDebugType != EDrawDebugTrace::None)
						{
							bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
							float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

							if (bHit && OutHits.Last().bBlockingHit)
							{
								// Red up to the blocking hit, green thereafter
								FVector const BlockingHitPoint = OutHits.Last().Location;
								DrawDebugSweptSphere(World, Start, BlockingHitPoint, Radius, TraceColor.ToFColor(true), bPersistent, LifeTime);
								DrawDebugSweptSphere(World, BlockingHitPoint, End, Radius, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
							}
							else
							{
								// no hit means all red
								DrawDebugSweptSphere(World, Start, End, Radius, TraceColor.ToFColor(true), bPersistent, LifeTime);
							}

							// draw hits
							for (int32 HitIdx = 0; HitIdx < OutHits.Num(); ++HitIdx)
							{
								FHitResult const& Hit = OutHits[HitIdx];
								::DrawDebugPoint(World, Hit.ImpactPoint, 16.f, (Hit.bBlockingHit ? TraceColor.ToFColor(true) : TraceHitColor.ToFColor(true)), bPersistent, LifeTime);
							}
						}
					}
					static void DrawDebugSweptSphere(const UWorld* InWorld, FVector const& Start, FVector const& End, float Radius, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority = 0)
					{
						FVector const TraceVec = End - Start;
						float const Dist = TraceVec.Size();

						FVector const Center = Start + TraceVec * 0.5f;
						float const HalfHeight = (Dist * 0.5f) + Radius;

						FQuat const CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
						::DrawDebugCapsule(InWorld, Center, HalfHeight, Radius, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);
					}
					void DrawDebugSweptBox(const UWorld* InWorld, FVector const& Start, FVector const& End, FRotator const & Orientation, FVector const & HalfSize, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority = 0)
					{
						FVector const TraceVec = End - Start;
						float const Dist = TraceVec.Size();

						FVector const Center = Start + TraceVec * 0.5f;

						FQuat const CapsuleRot = Orientation.Quaternion();
						::DrawDebugBox(InWorld, Start, HalfSize, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);

						//now draw lines from vertices
						FVector Vertices[8];
						Vertices[0] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, -HalfSize.Z));	//flt
						Vertices[1] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, HalfSize.Y, -HalfSize.Z));	//frt
						Vertices[2] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, HalfSize.Z));	//flb
						Vertices[3] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, HalfSize.Y, HalfSize.Z));	//frb
						Vertices[4] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, -HalfSize.Y, -HalfSize.Z));	//blt
						Vertices[5] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, HalfSize.Y, -HalfSize.Z));	//brt
						Vertices[6] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, -HalfSize.Y, HalfSize.Z));	//blb
						Vertices[7] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, HalfSize.Y, HalfSize.Z));		//brb
						for (int32 VertexIdx = 0; VertexIdx < 8; ++VertexIdx)
						{
							::DrawDebugLine(InWorld, Vertices[VertexIdx], Vertices[VertexIdx] + TraceVec, Color, bPersistentLines, LifeTime, DepthPriority);
						}

						::DrawDebugBox(InWorld, End, HalfSize, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);
					}
				};
				FKismetTraceUtils::DrawDebugSphereTraceMulti(GetWorld(), TraceStart, TraceEnd, Config->StepLength / 2.f, GetDebugType(), false, Hits, FColor::Red, FColor::Green, 0.2f);
				continue;
			}
#endif

			if (UKismetSystemLibrary::SphereTraceMulti(TargetSocketMesh.Get(), TraceStart, TraceEnd, Config->StepLength / 2.f, Config->TraceTypeQuery, true, TracedActors, GetDebugType(), Hits, true))
			{
				for (const FHitResult& E_Hit : Hits)
				{
					if (E_Hit.bBlockingHit && !TracedInOnceTraceActor.Contains(E_Hit.GetActor()))
					{
						TracedInOnceTraceActor.Add(E_Hit.GetActor());
						TracedActors.Add(E_Hit.GetActor());
						OnTraceActor.ExecuteIfBound(TargetSocketMesh.Get(), GetTraceSocketList()[i], E_Hit.GetActor(), E_Hit.GetComponent(), E_Hit);
						OnTraceActorNative.ExecuteIfBound(TargetSocketMesh.Get(), GetTraceSocketList()[i], E_Hit.GetActor(), E_Hit.GetComponent(), E_Hit);
					}
				}
			}
		}
	}

	//记录位置信息
	RecordPrePosition();
}

void USocketMoveTracer::RecordPrePosition()
{
	for (int i = 0; i < GetTraceSocketList().Num(); ++i)
	{
		PrePosList[i] = TargetSocketMesh->GetSocketLocation(GetTraceSocketList()[i]);
	}
}
