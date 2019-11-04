// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PropertyDef.h"

void UARPG_GameplayFloatPropertyOperatorBase::ApplyValue(EARPG_PropertyOperatorOperand Operand, UObject* Owner, float Value, const TSoftObjectPtr<const UObject>& InInstigator)
{
	switch (Operand)
	{
	case EARPG_PropertyOperatorOperand::Additive:
		AddValue(Owner, Value, InInstigator);
		break;
	case EARPG_PropertyOperatorOperand::Multiple:
		MultipleValue(Owner, Value, InInstigator);
		break;
	case EARPG_PropertyOperatorOperand::Set:
		SetValue(Owner, Value, InInstigator);
		break;
	}
}

void UARPG_GameplayFloatPropertyModifierBase::PushModifier(EARPG_PropertyModifierOperand Operand, UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	switch (Operand)
	{
	case EARPG_PropertyModifierOperand::Additive:
		PushAdditiveMultipleModifier(Owner, ModifyConfig);
		break;
	case EARPG_PropertyModifierOperand::Multiple:
		PushMultipleModifier(Owner, ModifyConfig);
		break;
	}
}

void UARPG_GameplayFloatPropertyModifierBase::PopModifier(EARPG_PropertyModifierOperand Operand, UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	switch (Operand)
	{
	case EARPG_PropertyModifierOperand::Additive:
		PopAdditiveMultipleModifier(Owner, ModifyConfig);
		break;
	case EARPG_PropertyModifierOperand::Multiple:
		PopMultipleModifier(Owner, ModifyConfig);
		break;
	}
}
