// Fill out your copyright notice in the Description page of Project Settings.

#include "AIPerceptionExLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"

class UAISenseConfig* UAIPerceptionExLibrary::GetSenseConfig(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> AISenseType)
{
	if (AIPerceptionComponent)
	{
		FAISenseID Id = UAISense::GetSenseID(AISenseType);
		if (Id.IsValid())
		{
			return AIPerceptionComponent->GetSenseConfig(Id);
		}
	}
	return nullptr;
}

const struct FAIStimulus* UAIPerceptionExLibrary::GetLastSensedStimuli(class UAIPerceptionComponent* AIPerceptionComponent, AActor* Actor, TSubclassOf<class UAISense> AISenseType)
{
	FAISenseID SightSenseID = UAISense::GetSenseID(AISenseType);
	if (const FActorPerceptionInfo* ActorPerceptionInfo = AIPerceptionComponent->GetActorInfo(*Actor))
	{
		return ActorPerceptionInfo->LastSensedStimuli.FindByPredicate([&](const FAIStimulus& E) {return E.Type == SightSenseID; });
	}
	return nullptr;
}

bool UAIPerceptionExLibrary::GetLastSensedStimuli(class UAIPerceptionComponent* AIPerceptionComponent, AActor* Actor, TSubclassOf<class UAISense> AISenseType, FAIStimulus& AIStimulus)
{
	if (const FAIStimulus* p_AIStimulus = GetLastSensedStimuli(AIPerceptionComponent, Actor, AISenseType))
	{
		AIStimulus = *p_AIStimulus;
		return true;
	}
	return false;
}

TArray<class AActor*> UAIPerceptionExLibrary::GetKnownPerceivedActorsEx(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> SenseToUse, TSubclassOf<class AActor> Type)
{
	TArray<AActor*> Res;
	if (AIPerceptionComponent)
	{
		TArray<AActor*> KnownPerceivedActors;
		AIPerceptionComponent->GetKnownPerceivedActors(SenseToUse, KnownPerceivedActors);

		for (AActor* KnownPerceivedActor : KnownPerceivedActors)
		{
			if (Type == nullptr || KnownPerceivedActor->IsA(Type))
			{
				Res.Add(KnownPerceivedActor);
			}
		}
	}
	return Res;
}

TArray<class AActor*> UAIPerceptionExLibrary::GetKnownPerceivedActorsByAge(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> SenseToUse, float MaxAge, TSubclassOf<class AActor> Type)
{
	TArray<AActor*> Res;
	if (AIPerceptionComponent)
	{
		TArray<AActor*> KnownPerceivedActors = GetKnownPerceivedActorsEx(AIPerceptionComponent, SenseToUse, Type);

		for (AActor* KnownPerceivedActor : KnownPerceivedActors)
		{
			const FActorPerceptionInfo* ActorPerceptionInfo = AIPerceptionComponent->GetActorInfo(*KnownPerceivedActor);

			if (const FAIStimulus* AIStimulus = GetLastSensedStimuli(AIPerceptionComponent, KnownPerceivedActor, SenseToUse))
			{
				if (AIStimulus->GetAge() < MaxAge)
				{
					Res.Add(KnownPerceivedActor);
				}
			}
		}
	}
	return Res;
}
