// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PropertyDef.h"

void FARPG_FloatProperty::PushAdditiveModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	Additive += ModifyConfig.Value;
	UpdateFinalValue();
}

void FARPG_FloatProperty::PopAdditiveModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	Additive -= ModifyConfig.Value;
	UpdateFinalValue();
}

void FARPG_FloatProperty::ChangePushedAdditiveModifier(const FARPG_FloatProperty_ModifyConfig& OldModifyConfig, const FARPG_FloatProperty_ModifyConfig& ChangeToModifyConfig)
{
	Additive += ChangeToModifyConfig.Value - OldModifyConfig.Value;
	UpdateFinalValue();
}

void FARPG_FloatProperty::PushMultipleModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	Multiple += ModifyConfig.Value;
	UpdateFinalValue();
}

void FARPG_FloatProperty::PopMultipleModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	Multiple -= ModifyConfig.Value;
	UpdateFinalValue();
}

void FARPG_FloatProperty::ChangePushedMultipleModifier(const FARPG_FloatProperty_ModifyConfig& OldModifyConfig, const FARPG_FloatProperty_ModifyConfig& ChangeToModifyConfig)
{
	Multiple += ChangeToModifyConfig.Value - OldModifyConfig.Value;
	UpdateFinalValue();
}
