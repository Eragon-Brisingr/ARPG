// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PropertyOperator_Impl.h"
#include "CharacterBase.h"

float UARPG_Health_PropertyOperator::GetValue(const UObject* Owner) const
{
	return CastChecked<ACharacterBase>(Owner)->GetHealth();
}

void UARPG_Health_PropertyOperator::SetValue(UObject* Owner, float InValue, const UObject* InInstigator) const
{
	CastChecked<ACharacterBase>(Owner)->SetHealth(InValue, InInstigator);
}

float UARPG_MaxHealth_PropertyOperator::GetValue(const UObject* Owner) const
{
	return CastChecked<ACharacterBase>(Owner)->GetMaxHealth();
}

void UARPG_MaxHealth_PropertyOperator::SetValue(UObject* Owner, float InValue, const UObject* InInstigator) const
{
	unimplemented();
}

void UARPG_MaxHealth_PropertyOperator::PushAdditiveMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHelath.PushAdditiveMultipleModifier(ModifyConfig);
}

void UARPG_MaxHealth_PropertyOperator::PopAdditiveMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHelath.PopAdditiveMultipleModifier(ModifyConfig);
}

void UARPG_MaxHealth_PropertyOperator::PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHelath.PushMultipleModifier(ModifyConfig);
}

void UARPG_MaxHealth_PropertyOperator::PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHelath.PopMultipleModifier(ModifyConfig);
}
