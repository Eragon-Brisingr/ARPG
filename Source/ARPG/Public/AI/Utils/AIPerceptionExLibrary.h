// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SubclassOf.h"
#include "XD_TemplateLibrary.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIPerceptionExLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UAIPerceptionExLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//修改完别忘记调用RequestStimuliListenerUpdate
	UFUNCTION(BlueprintCallable, Category = "AI|Perception")
	static class UAISenseConfig* GetSenseConfig(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> AISenseType);

	UFUNCTION(BlueprintCallable, Category = "AI|Perception")
	static bool GetLastSensedStimuli(class UAIPerceptionComponent* AIPerceptionComponent, AActor* Actor, TSubclassOf<class UAISense> AISenseType, FAIStimulus& AIStimulus);
	static const struct FAIStimulus* GetLastSensedStimuli(class UAIPerceptionComponent* AIPerceptionComponent, AActor* Actor, TSubclassOf<class UAISense> AISenseType);

	UFUNCTION(BlueprintCallable, Category = "AI|Perception", meta = (DeterminesOutputType = "Type"))
	static TArray<class AActor*> GetKnownPerceivedActorsEx(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> SenseToUse, TSubclassOf<class AActor> Type);
	template<typename T>
	static TArray<T*> GetKnownPerceivedActorsEx(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> SenseToUse)
	{
		return ArrayCast<T*>(GetKnownPerceivedActorsEx(AIPerceptionComponent, SenseToUse, T::StaticClass()));
	}

	UFUNCTION(BlueprintCallable, Category = "AI|Perception", meta = (DeterminesOutputType = "Type"))
	static TArray<class AActor*> GetCurrentlyPerceivedActorsEx(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> SenseToUse, TSubclassOf<class AActor> Type);
	template<typename T>
	static TArray<T*> GetCurrentlyPerceivedActorsEx(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> SenseToUse)
	{
		return ArrayCast<T*>(GetCurrentlyPerceivedActorsEx(AIPerceptionComponent, SenseToUse, T::StaticClass()));
	}

	UFUNCTION(BlueprintCallable, Category = "AI|Perception")
	static bool GetActorLastStimulusLocation(class UAIPerceptionComponent* AIPerceptionComponent, class AActor* Actor, FVector& Location, float& Age);
	//过滤器
public:
	UFUNCTION(BlueprintCallable, Category = "AI|Perception", meta = (DeterminesOutputType = "PerceivedActors"))
	static TArray<class AActor*> FilterPerceivedActorsByMaxAge(class UAIPerceptionComponent* AIPerceptionComponent, const TArray<AActor*>& PerceivedActors, TSubclassOf<class UAISense> SenseToUse, float MaxAge);
	template<typename T>
	static TArray<T*> FilterPerceivedActorsByMaxAge(class UAIPerceptionComponent* AIPerceptionComponent, const TArray<T*>& PerceivedActors, TSubclassOf<class UAISense> SenseToUse, float MaxAge)
	{
		return ArrayCast<T*>(FilterPerceivedActorsByMaxAge(AIPerceptionComponent, ArrayCast<AActor*>(PerceivedActors), SenseToUse, MaxAge));
	}

	UFUNCTION(BlueprintCallable, Category = "AI|Perception", meta = (DeterminesOutputType = "PerceivedActors"))
	static TArray<class AActor*> FilterPerceivedActorsByTag(class UAIPerceptionComponent* AIPerceptionComponent, const TArray<AActor*>& PerceivedActors, TSubclassOf<class UAISense> SenseToUse, FName Tag);
	template<typename T>
	static TArray<T*> FilterPerceivedActorsByTag(class UAIPerceptionComponent* AIPerceptionComponent, const TArray<T*>& PerceivedActors, TSubclassOf<class UAISense> SenseToUse, const FName& Tag)
	{
		return ArrayCast<T*>(FilterPerceivedActorsByTag(AIPerceptionComponent, ArrayCast<AActor*>(PerceivedActors), Tag, MaxAge, T::StaticClass()));
	}
};
