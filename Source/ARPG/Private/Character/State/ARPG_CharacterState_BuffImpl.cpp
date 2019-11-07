// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_CharacterState_BuffImpl.h"
#include "ARPG_PropertyOperator.h"
#include "CharacterBase.h"

void UARPG_CS_Buff_General::WhenActived(bool IsFirstInit)
{
	for (FGeneralBuffModifierConfig& Config : EffectModifiers)
	{
		if (Config.ModifyProperty)
		{
			UARPG_GameplayFloatPropertyModifierBase* PropertyModifier = Config.ModifyProperty.GetDefaultObject();

			PropertyModifier->PushModifier(Config.Operand, Owner, FARPG_FloatProperty_ModifyConfig(Config.Value, GetFName(), this, Instigator));
		}
	}

	for (FGeneralBuffOperatorConfig& Config : ActiveOperators)
	{
		if (Config.OperatorProperty)
		{
			UARPG_GameplayFloatPropertyOperatorBase* OperatorProperty = Config.OperatorProperty.GetDefaultObject();

			OperatorProperty->ApplyValue(Config.Operand, Owner, Config.Value, FARPG_PropertyChangeContext(this, Instigator));
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

			OperatorProperty->ApplyValue(Config.Operand, Owner, Config.Value * DeltaTime, FARPG_PropertyChangeContext(this, Instigator));
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

			PropertyModifier->PopModifier(Config.Operand, Owner, FARPG_FloatProperty_ModifyConfig(Config.Value, GetFName(), this, Instigator));
		}
	}

	for (FGeneralBuffOperatorConfig& Config : DeactiveOperators)
	{
		if (Config.OperatorProperty)
		{
			UARPG_GameplayFloatPropertyOperatorBase* OperatorProperty = Config.OperatorProperty.GetDefaultObject();

			OperatorProperty->ApplyValue(Config.Operand, Owner, Config.Value, FARPG_PropertyChangeContext(this, Instigator));
		}
	}

	Super::WhenDeactived();
}

void UARPG_CS_Buff_General::WhenRepeatActive()
{
	switch (GeneralBuffOverlayType)
	{
	case EGeneralBuffOverlayType::Replace:
		ContinuedTime = 0.f;
		break;
	case EGeneralBuffOverlayType::ExtendTime:
		DurationTime += GetClass()->GetDefaultObject<UARPG_CS_Buff_General>()->DurationTime;
		break;
	}

	Super::WhenRepeatActive();
}
