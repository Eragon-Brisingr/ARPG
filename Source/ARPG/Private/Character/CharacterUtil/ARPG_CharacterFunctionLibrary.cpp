// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterFunctionLibrary.h"
#include "CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"




bool UARPG_CharacterFunctionLibrary::IsInDangerousArea(class ACharacterBase* Asker, float TraceRadius)
{
	if (Asker)
	{
		for (ACharacterBase* Character : Asker->GetSeeingTraceEnemies())
		{
			for (const FARPG_AttackInfo& AttackInfo : Character->AttackInfos)
			{
				float Distance = (Asker->GetActorLocation() - Character->GetMesh()->GetSocketTransform(AttackInfo.SocketName).TransformPosition(AttackInfo.Offset)).Size();
				float Radius = AttackInfo.Radius + Asker->GetCapsuleComponent()->GetScaledCapsuleRadius();
				if (Distance < Radius)
				{
					return true;
				}
			}
		}
	}
	return false;
}
