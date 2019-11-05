// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PropertyOperator_Impl.h"
#include "CharacterBase.h"
#include "ARPG_ReceiveDamageActionBase.h"

float UARPG_Health_PropertyOperator::GetValue(const UObject* Owner) const
{
	return CastChecked<ACharacterBase>(Owner)->GetHealth();
}

void UARPG_Health_PropertyOperator::SetValue(UObject* Owner, float InValue, const FARPG_PropertyChangeContext& ChangeContext) const
{
	ACharacterBase* Character = CastChecked<ACharacterBase>(Owner);
	if (Character->IsAlive())
	{
		Character->SetHealth(InValue, ChangeContext);

		if (Character->IsDead())
		{
			if (UARPG_ReceiveDamageActionBase* ReceiveDamageAction = Character->GetReceiveDamageAction())
			{
				ReceiveDamageAction->PlayNormalDeadMontage(Character);
			}
		}
	}
}

float UARPG_MaxHealth_PropertyModifier::GetValue(const UObject* Owner) const
{
	return CastChecked<ACharacterBase>(Owner)->GetMaxHealth();
}

void UARPG_MaxHealth_PropertyModifier::PushAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHealth_PushAdditiveModifier(ModifyConfig);
}

void UARPG_MaxHealth_PropertyModifier::PopAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHealth_PopAdditiveModifier(ModifyConfig);
}

void UARPG_MaxHealth_PropertyModifier::PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHealth_PushMultipleModifier(ModifyConfig);
}

void UARPG_MaxHealth_PropertyModifier::PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHealth_PopMultipleModifier(ModifyConfig);
}

float UARPG_Stamina_PropertyOperator::GetValue(const UObject* Owner) const
{
	return CastChecked<ACharacterBase>(Owner)->GetStamina();
}

void UARPG_Stamina_PropertyOperator::SetValue(UObject* Owner, float InValue, const FARPG_PropertyChangeContext& ChangeContext) const
{
	CastChecked<ACharacterBase>(Owner)->SetStamina(InValue, ChangeContext);
}

float UARPG_MaxStamina_PropertyModifier::GetValue(const UObject* Owner) const
{
	return CastChecked<ACharacterBase>(Owner)->GetMaxStamina();
}

void UARPG_MaxStamina_PropertyModifier::PushAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxStamina_PushAdditiveModifier(ModifyConfig);
}

void UARPG_MaxStamina_PropertyModifier::PopAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxStamina_PopAdditiveModifier(ModifyConfig);
}

void UARPG_MaxStamina_PropertyModifier::PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxStamina_PushMultipleModifier(ModifyConfig);
}

void UARPG_MaxStamina_PropertyModifier::PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxStamina_PopMultipleModifier(ModifyConfig);
}
