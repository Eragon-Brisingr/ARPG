// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_CharacterState_BuffImpl.h"
#include "ARPG_PropertyDef.h"
#include "CharacterBase.h"

void UARPG_CS_Buff_General::WhenActived(bool IsFirstInit)
{
	for (FGeneralBuffModifierConfig& Config : EffectModifiers)
	{
		if (Config.ModifyProperty)
		{
			UARPG_GameplayFloatPropertyModifierBase* PropertyModifier = Config.ModifyProperty.GetDefaultObject();

			FARPG_FloatProperty_ModifyConfig ModifyConfig;
			ModifyConfig.Value = Config.Value;
			ModifyConfig.Instigator = Instigator;
			ModifyConfig.DescribeTag = GetFName();

			PropertyModifier->PushModifier(Config.Operand, Owner, ModifyConfig);
		}
	}

	for (FGeneralBuffOperatorConfig& Config : ActiveOperators)
	{
		if (Config.OperatorProperty)
		{
			UARPG_GameplayFloatPropertyOperatorBase* OperatorProperty = Config.OperatorProperty.GetDefaultObject();

			OperatorProperty->ApplyValue(Config.Operand, Owner, Config.Value, Instigator);
		}
	}

	Super::WhenActived(IsFirstInit);
}

void UARPG_CS_Buff_General::WhenTick(float DeltaTime)
{
	for (FGeneralBuffOperatorConfig& Config : TickOperators)
	{
		if (Config.OperatorProperty)
		{
			UARPG_GameplayFloatPropertyOperatorBase* OperatorProperty = Config.OperatorProperty.GetDefaultObject();

			OperatorProperty->ApplyValue(Config.Operand, Owner, Config.Value, Instigator);
		}
	}

	Super::WhenTick(DeltaTime);
}

void UARPG_CS_Buff_General::WhenDeactived()
{
	for (FGeneralBuffModifierConfig& Config : EffectModifiers)
	{
		if (Config.ModifyProperty)
		{
			UARPG_GameplayFloatPropertyModifierBase* PropertyModifier = Config.ModifyProperty.GetDefaultObject();

			FARPG_FloatProperty_ModifyConfig ModifyConfig;
			ModifyConfig.Value = Config.Value;
			ModifyConfig.Instigator = Instigator;
			ModifyConfig.DescribeTag = GetFName();

			PropertyModifier->PopModifier(Config.Operand, Owner, ModifyConfig);
		}
	}

	for (FGeneralBuffOperatorConfig& Config : DeactiveOperators)
	{
		if (Config.OperatorProperty)
		{
			UARPG_GameplayFloatPropertyOperatorBase* OperatorProperty = Config.OperatorProperty.GetDefaultObject();

			OperatorProperty->ApplyValue(Config.Operand, Owner, Config.Value, Instigator.Get());
		}
	}

	Super::WhenDeactived();
}
