// Fill out your copyright notice in the Description page of Project Settings.

#include "SocketMoveTraceManager.h"
#include <Components/PrimitiveComponent.h>
#include "ARPG_CollisionType.h"



USocketMoveTracer::USocketMoveTracer()
	:TraceTypeQuery(FARPG_TraceTypeQuery::Visibility)
{

}

void USocketMoveTracer::Tick(float DeltaTime)
{
	if (TargetSocketMesh.IsValid())
	{
		DoTrace(DeltaTime);
	}
}

UWorld* USocketMoveTracer::GetTickableGameObjectWorld() const
{
	return TargetSocketMesh.IsValid() ? TargetSocketMesh->GetWorld() : nullptr;
}

AActor* USocketMoveTracer::GetOwner() const
{
	return TargetSocketMesh.IsValid() ? TargetSocketMesh->GetOwner() : nullptr;
}

void USocketMoveTracer::InitSocketMoveTracer(UPrimitiveComponent* TargetComponent)
{
	if (TargetComponent)
	{
		TargetSocketMesh = TargetComponent;
		TracedActors.Append({ GetOwner(), GetOwner()->GetOwner() });
		if (TraceSocketList.Num() < 2)
		{
			TraceSocketList = TargetSocketMesh->GetAllSocketNames();
		}
		PrePosList.SetNum(TraceSocketList.Num());
	}
}

void USocketMoveTracer::EnableTrace(bool ClearIgnoreList /*= true*/)
{
	if (bEnableTrace != true)
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
	TArray<AActor*> TracedInOnceTraceActor;
	for (int i = 1; i < TraceSocketList.Num(); ++i)
	{
		float StepAddAlpha = StepLength / (PrePosList[i - 1] - PrePosList[i]).Size();

		FVector TraceStart_Start = PrePosList[i - 1];
		FVector TraceStart_End = PrePosList[i];
		FVector TraceEnd_Start = TargetSocketMesh->GetSocketLocation(TraceSocketList[i - 1]);
		FVector TraceEnd_End = TargetSocketMesh->GetSocketLocation(TraceSocketList[i]);

		for (float Alpha = 0; Alpha < 1.f; Alpha += StepAddAlpha)
		{
			FVector TraceStart = FMath::Lerp(TraceStart_Start, TraceStart_End, Alpha);
			FVector TraceEnd = FMath::Lerp(TraceEnd_Start, TraceEnd_End, Alpha);

			TArray<FHitResult> Hits;

			if (UKismetSystemLibrary::SphereTraceMulti(TargetSocketMesh.Get(), TraceStart, TraceEnd, StepLength / 2.f, TraceTypeQuery, true, TracedActors, GetDebugType(), Hits, true))
			{
				for (const FHitResult& E_Hit : Hits)
				{
					if (E_Hit.bBlockingHit && !TracedInOnceTraceActor.Contains(E_Hit.GetActor()))
					{
						TracedInOnceTraceActor.Add(E_Hit.GetActor());
						TracedActors.Add(E_Hit.GetActor());
						OnTraceActor.ExecuteIfBound(TargetSocketMesh.Get(), TraceSocketList[i], E_Hit.GetActor(), E_Hit.GetComponent(), E_Hit);
						OnTraceActorNative.ExecuteIfBound(TargetSocketMesh.Get(), TraceSocketList[i], E_Hit.GetActor(), E_Hit.GetComponent(), E_Hit);
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
	for (int i = 0; i < TraceSocketList.Num(); ++i)
	{
		PrePosList[i] = TargetSocketMesh->GetSocketLocation(TraceSocketList[i]);
	}
}

void USocketMoveTracer::SetTraceSocketList(const TArray<FName>& SocketList)
{
	TraceSocketList = SocketList;
	PrePosList.SetNum(TraceSocketList.Num());
}

void USocketMoveTracer::ClearTraceSocketList()
{
	TraceSocketList.Empty();
	PrePosList.Empty();
}

void USocketMoveTracer::SetEnableTrace(bool Enable)
{
	if (bEnableTrace)
	{
		EnableTrace();
	}
	else
	{
		DisableTrace();
	}
}
