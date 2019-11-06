// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PropertyOperator.h"

void UARPG_GameplayFloatPropertyOperatorBase::ApplyValue(EARPG_PropertyOperatorOperand Operand, UObject* Owner, float Value, const FARPG_PropertyChangeContext& ChangeContext)
{
	switch (Operand)
	{
	case EARPG_PropertyOperatorOperand::Additive:
		AddValue(Owner, Value, ChangeContext);
		break;
	case EARPG_PropertyOperatorOperand::Multiple:
		MultipleValue(Owner, Value, ChangeContext);
		break;
	case EARPG_PropertyOperatorOperand::Set:
		SetValue(Owner, Value, ChangeContext);
		break;
	}
}

void UARPG_GameplayFloatPropertyModifierBase::PushModifier(EARPG_PropertyModifierOperand Operand, UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	switch (Operand)
	{
	case EARPG_PropertyModifierOperand::Additive:
		PushAdditiveModifier(Owner, ModifyConfig);
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
		PopAdditiveModifier(Owner, ModifyConfig);
		break;
	case EARPG_PropertyModifierOperand::Multiple:
		PopMultipleModifier(Owner, ModifyConfig);
		break;
	}
}

