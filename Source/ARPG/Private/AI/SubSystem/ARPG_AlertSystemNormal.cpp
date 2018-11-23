// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AlertSystemNormal.h"
#include "CharacterBase.h"
#include "ARPG_SneakSystemBase.h"

float UARPG_AlertSystemNormal::GetSightAddAlertValue(const class ACharacterBase* LookedCharacter) const
{
	if (LookedCharacter)
	{
		//估计距离 = Distance * TargetHideLevel / VigilanceRate
		return SightAlertCurve.GetRichCurveConst()->Eval(Character->GetDistanceTo(LookedCharacter) * LookedCharacter->SneakSystem->GetSightHideValue() / AlertRate);
	}
	return 0.f;
}

float UARPG_AlertSystemNormal::GetHearAddAlertValue(const class ACharacterBase* HeardCharacter, const FVector& SimulusLocation, float Strength) const
{
	if (HeardCharacter)
	{
		//估计距离 = Distance * TargetHideLevel / SoundStrength / VigilanceRate
		return HearAlertCurve.GetRichCurveConst()->Eval((SimulusLocation - Character->GetActorLocation()).Size() * HeardCharacter->SneakSystem->GetHearHideValue() / Strength / AlertRate);
	}
	return 0.f;
}

float UARPG_AlertSystemNormal::GetNoseAddAlertValue(const class ACharacterBase* NosedCharacter) const
{
	if (NosedCharacter)
	{
		//待定、考虑风向？
		return NoseAlertCurve.GetRichCurveConst()->Eval(Character->GetDistanceTo(NosedCharacter) * NosedCharacter->SneakSystem->GetNoseHideValue() / AlertRate);
	}
	return 0.f;
}
