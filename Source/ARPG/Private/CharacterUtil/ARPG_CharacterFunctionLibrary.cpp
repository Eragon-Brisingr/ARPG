// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterFunctionLibrary.h"
#include "CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"




bool UARPG_CharacterFunctionLibrary::IsInDamageArea(class ACharacterBase* Asker, float TraceRadius /*= 500.f*/)
{
	if (Asker)
	{
		TArray<AActor*> OverlapedActors;
		if (UKismetSystemLibrary::SphereOverlapActors(Asker, Asker->GetActorLocation(), TraceRadius, { FARPG_CollisionObjectType::Pawn }, {}, { Asker }, OverlapedActors))
		{
			for (AActor* Actor : OverlapedActors)
			{
				if (ACharacterBase* Character = Cast<ACharacterBase>(Actor))
				{
					for (const FARPG_AttackInfo& AttackInfo : Character->AttackInfos)
					{
						if ((Asker->GetActorLocation() - Character->GetMesh()->GetSocketLocation(AttackInfo.SocketName)).Size() < Asker->GetCapsuleComponent()->GetScaledCapsuleRadius())
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
