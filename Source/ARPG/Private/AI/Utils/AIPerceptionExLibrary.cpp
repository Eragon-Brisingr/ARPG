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

template<typename TPredicate>
TArray<class AActor*> FilterActorsByStimulus(const TArray<AActor*>& PerceivedActors, class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> SenseToUse, const TPredicate& Predicate)
{
	TArray<AActor*> Res;
	for (AActor* KnownPerceivedActor : PerceivedActors)
	{
		const FActorPerceptionInfo* ActorPerceptionInfo = AIPerceptionComponent->GetActorInfo(*KnownPerceivedActor);

		if (const FAIStimulus* AIStimulus = UAIPerceptionExLibrary::GetLastSensedStimuli(AIPerceptionComponent, KnownPerceivedActor, SenseToUse))
		{
			if (Predicate(*AIStimulus))
			{
				Res.Add(KnownPerceivedActor);
			}
		}
	}

	return Res;
}

TArray<class AActor*> UAIPerceptionExLibrary::FilterPerceivedActorsByMaxAge(class UAIPerceptionComponent* AIPerceptionComponent, const TArray<AActor*>& PerceivedActors, TSubclassOf<class UAISense> SenseToUse, float MaxAge)
{
	if (AIPerceptionComponent)
	{
		return FilterActorsByStimulus(PerceivedActors, AIPerceptionComponent, SenseToUse, [MaxAge](const FAIStimulus& AIStimulus) {return AIStimulus.GetAge() < MaxAge; });
	}
	return {};
}

TArray<class AActor*> UAIPerceptionExLibrary::FilterPerceivedActorsByTag(class UAIPerceptionComponent* AIPerceptionComponent, const TArray<AActor*>& PerceivedActors, TSubclassOf<class UAISense> SenseToUse, FName Tag)
{
	if (AIPerceptionComponent)
	{
		return FilterActorsByStimulus(PerceivedActors, AIPerceptionComponent, SenseToUse, [&](const FAIStimulus& AIStimulus) {return AIStimulus.Tag == Tag; });
	}
	return {};
}
