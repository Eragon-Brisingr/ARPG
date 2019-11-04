// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_CharacterState_BuffImpl.h"
#include "ARPG_PropertyDef.h"
#include "CharacterBase.h"

void UARPG_CS_Buff_General::WhenActived(bool IsFirstInit)
{
	for (FGeneralBuffPropertyConfig& Config : EffectPropertys)
	{
		if (Config.ModifyProperty)
		{
			UARPG_GameplayFloatPropertyModifierBase* PropertyModifier = Config.ModifyProperty.GetDefaultObject();

			FARPG_FloatProperty_ModifyConfig ModifyConfig;
			ModifyConfig.Value = Config.Value;
			ModifyConfig.Instigator = Instigator;
			ModifyConfig.DescribeTag = GetFName();

			switch (Config.Operand)
			{
			case EGeneralBuffPropertyOperand::Additive:
				PropertyModifier->PushAdditiveMultipleModifier(Owner, ModifyConfig);
				break;
			case EGeneralBuffPropertyOperand::Multiple:
				PropertyModifier->PushMultipleModifier(Owner, ModifyConfig);
				break;
			}
		}
	}
}

void UARPG_CS_Buff_General::WhenTick(float DeltaTime)
{

}

void UARPG_CS_Buff_General::WhenDeactived()
{
	for (FGeneralBuffPropertyConfig& Config : EffectPropertys)
	{
		if (Config.ModifyProperty)
		{
			UARPG_GameplayFloatPropertyModifierBase* PropertyModifier = Config.ModifyProperty.GetDefaultObject();

			FARPG_FloatProperty_ModifyConfig ModifyConfig;
			ModifyConfig.Value = Config.Value;
			ModifyConfig.Instigator = Instigator;
			ModifyConfig.DescribeTag = GetFName();

			switch (Config.Operand)
			{
			case EGeneralBuffPropertyOperand::Additive:
				PropertyModifier->PopAdditiveMultipleModifier(Owner, ModifyConfig);
				break;
			case EGeneralBuffPropertyOperand::Multiple:
				PropertyModifier->PopMultipleModifier(Owner, ModifyConfig);
				break;
			}
		}
	}
}
