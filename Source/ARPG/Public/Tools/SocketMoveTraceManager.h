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

USTRUCT(BlueprintType)
struct FSocketMoveTracerConfig
{
	GENERATED_BODY()
public:
	FSocketMoveTracerConfig();

	UPROPERTY(EditAnywhere, Category = "攻击检测", meta = (DisplayName = "检测步长"))
	float StepLength = 20.f;

	UPROPERTY(EditAnywhere, Category = "攻击检测", meta = (DisplayName = "插槽列表"))
	TArray<FName> TraceSocketList;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "攻击检测", meta = (DisplayName = "调试模式"))
	TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None;
#endif

	UPROPERTY(EditAnywhere, Category = "攻击检测", meta = (DisplayName = "检测碰撞通道"))
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;
};

UCLASS(editinlinenew, collapseCategories)
class ARPG_API USocketMoveTracer : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	USocketMoveTracer();

	void Tick(float DeltaTime) override;

	TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(USocketMoveTracer, STATGROUP_Tickables); }

	bool IsTickable() const override { return bEnableTrace; }

	UWorld* GetTickableGameObjectWorld() const override;

	UWorld* GetWorld() const override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "攻击检测")
	uint8 bEnableTrace : 1;

	FSocketMoveTracerConfig* Config;

	TWeakObjectPtr<UPrimitiveComponent> TargetSocketMesh;

	TArray<FVector> PrePosList;

	const TArray<FName>& GetTraceSocketList() const { return Config->TraceSocketList; }

	FORCEINLINE TEnumAsByte<EDrawDebugTrace::Type> GetDebugType() const
	{
#if WITH_EDITORONLY_DATA
		return Config->DebugType;
#endif
		return EDrawDebugTrace::None;
	}

	UPROPERTY()
	TArray<AActor*> TracedActors;
public:
	FOnTraceActor OnTraceActor;
	FOnTraceActorNative OnTraceActorNative;

	AActor* GetOwner() const;

	UFUNCTION(BlueprintCallable, Category = "游戏|检测")
	void InitSocketMoveTracer(UPrimitiveComponent* TargetComponent, FSocketMoveTracerConfig& SocketMoveTracerConfig);

	UFUNCTION(BlueprintCallable, Category = "游戏|检测")
	void EnableTrace(bool ClearIgnoreList = true);

	UFUNCTION(BlueprintCallable, Category = "游戏|检测")
	void DisableTrace();

	void DoTrace(float DeltaTime);

	void RecordPrePosition();
};
