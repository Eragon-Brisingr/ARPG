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
	static TArray<class AActor*> GetKnownPerceivedActorsByAge(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> SenseToUse, float MaxAge, TSubclassOf<class AActor> Type);
	template<typename T>
	static TArray<T*> GetKnownPerceivedActorsByAge(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> SenseToUse, float MaxAge)
	{
		return ArrayCast<T*>(GetKnownPerceivedActorsByAge(AIPerceptionComponent, SenseToUse, MaxAge, T::StaticClass()));
	}
};
