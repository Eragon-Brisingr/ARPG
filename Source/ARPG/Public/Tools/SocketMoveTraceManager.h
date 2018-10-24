// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Object.h>
#include <Tickable.h>
#include <Kismet/KismetSystemLibrary.h>
#include "SocketMoveTraceManager.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_FiveParams(FOnTraceActor, UPrimitiveComponent*, HitComponent, const FName&, SocketName, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, const FHitResult&, TraceResult);

DECLARE_DELEGATE_FiveParams(FOnTraceActorNative, UPrimitiveComponent*, const FName&, AActor*, UPrimitiveComponent*, const FHitResult&);

UCLASS(editinlinenew, collapseCategories)
class ARPG_API USocketMoveTracer : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	USocketMoveTracer();

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(USocketMoveTracer, STATGROUP_Tickables); }

	virtual bool IsTickable() const override { return bEnableTrace; }

	virtual UWorld* GetTickableGameObjectWorld() const;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "攻击检测")
	uint8 bEnableTrace : 1;

	TWeakObjectPtr<UPrimitiveComponent> TargetSocketMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "攻击检测")
	float StepLength = 20.f;

	TArray<FVector> PrePosList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "攻击检测")
	TArray<FName> TraceSocketList;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "攻击检测")
	TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None;
#endif

	FORCEINLINE TEnumAsByte<EDrawDebugTrace::Type> GetDebugType() const
	{
#if WITH_EDITORONLY_DATA
		return DebugType;
#endif
		return EDrawDebugTrace::None;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "攻击检测")
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;

	UPROPERTY()
	TArray<AActor*> TracedActors;
public:
	FOnTraceActor OnTraceActor;
	FOnTraceActorNative OnTraceActorNative;

	AActor* GetOwner() const;

	UFUNCTION(BlueprintCallable, Category = "游戏|检测")
	void InitSocketMoveTracer(UPrimitiveComponent* TargetComponent);

	void EnableTrace(bool ClearIgnoreList = true);

	void DisableTrace();

	void DoTrace(float DeltaTime);

	void RecordPrePosition();

	void SetTraceSocketList(const TArray<FName>& SocketList);

	void ClearTraceSocketList();

	UFUNCTION(BlueprintCallable, Category = "游戏|检测")
	void SetEnableTrace(bool Enable);
};
