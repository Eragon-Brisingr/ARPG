// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PropertyOperator_Impl.h"
#include "CharacterBase.h"
#include "ARPG_ReceiveDamageActionBase.h"

#define LOCTEXT_NAMESPACE "ARPG_GameplayProperty"

FText UARPG_Health_PropertyOperator::GetPropertyName() const
{
	return LOCTEXT("当前生命", "当前生命");
}

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

FText UARPG_MaxHealth_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("生命上限", "生命上限");
}

float UARPG_MaxHealth_PropertyModifier::GetValue(const UObject* Owner) const
{
	return CastChecked<ACharacterBase>(Owner)->GetMaxHealth();
}

void UARPG_MaxHealth_PropertyModifier::PushAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	ACharacterBase* Character = CastChecked<ACharacterBase>(Owner);
	bool PrevIsAlive = Character->IsAlive();
	Character->MaxHealth_PushAdditiveModifier(ModifyConfig);
	if (PrevIsAlive && Character->IsDead())
	{
		if (UARPG_ReceiveDamageActionBase* ReceiveDamageAction = Character->GetReceiveDamageAction())
		{
			ReceiveDamageAction->PlayNormalDeadMontage(Character);
		}
	}
}

void UARPG_MaxHealth_PropertyModifier::PopAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	ACharacterBase* Character = CastChecked<ACharacterBase>(Owner);
	bool PrevIsAlive = Character->IsAlive();
	CastChecked<ACharacterBase>(Owner)->MaxHealth_PopAdditiveModifier(ModifyConfig);
	if (PrevIsAlive && Character->IsDead())
	{
		if (UARPG_ReceiveDamageActionBase* ReceiveDamageAction = Character->GetReceiveDamageAction())
		{
			ReceiveDamageAction->PlayNormalDeadMontage(Character);
		}
	}
}

void UARPG_MaxHealth_PropertyModifier::PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHealth_PushMultipleModifier(ModifyConfig);
}

void UARPG_MaxHealth_PropertyModifier::PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
{
	CastChecked<ACharacterBase>(Owner)->MaxHealth_PopMultipleModifier(ModifyConfig);
}

FText UARPG_Stamina_PropertyOperator::GetPropertyName() const
{
	return LOCTEXT("当前精力", "当前精力");
}

float UARPG_Stamina_PropertyOperator::GetValue(const UObject* Owner) const
{
	return CastChecked<ACharacterBase>(Owner)->GetStamina();
}

void UARPG_Stamina_PropertyOperator::SetValue(UObject* Owner, float InValue, const FARPG_PropertyChangeContext& ChangeContext) const
{
	CastChecked<ACharacterBase>(Owner)->SetStamina(InValue, ChangeContext);
}

FText UARPG_MaxStamina_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("精力上限", "精力上限");
}

ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(UARPG_MaxStamina_PropertyModifier, ACharacterBase, MaxStamina);

FText UARPG_StaminaRestoreSpeed_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("精力恢复速度", "精力恢复速度");
}

ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(UARPG_StaminaRestoreSpeed_PropertyModifier, ACharacterBase, StaminaRestoreSpeed);

FText UARPG_StaminaRestoreCoolDownTime_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("精力再恢复冷却时间", "精力再恢复冷却时间");
}

ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(UARPG_StaminaRestoreCoolDownTime_PropertyModifier, ACharacterBase, StaminaRestoreCoolDownTime);

FText UARPG_SprintStaminaReduceSpeed_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("冲刺精力消耗速度", "冲刺精力消耗速度");
}

ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(UARPG_SprintStaminaReduceSpeed_PropertyModifier, ACharacterBase, SprintStaminaReduceSpeed);

FText UARPG_Bearload_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("当前总计负重", "当前总计负重");
}

ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(UARPG_Bearload_PropertyModifier, ACharacterBase, Bearload);

FText UARPG_MaxBearload_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("总计负重上限", "总计负重上限");
}

ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(UARPG_MaxBearload_PropertyModifier, ACharacterBase, MaxBearload);

FText UARPG_EquipLoad_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("当前装备负重", "当前装备负重");
}

ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(UARPG_EquipLoad_PropertyModifier, ACharacterBase, EquipLoad);

FText UARPG_MaxEquipLoad_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("装备负重上限", "装备负重上限");
}

ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(UARPG_MaxEquipLoad_PropertyModifier, ACharacterBase, MaxEquipLoad);

FText UARPG_HitStunValue_PropertyOperator::GetPropertyName() const
{
	return LOCTEXT("削韧积累值", "削韧积累值");
}

float UARPG_HitStunValue_PropertyOperator::GetValue(const UObject* Owner) const
{
	return CastChecked<ACharacterBase>(Owner)->GetHitStunValue();
}

FText UARPG_Toughness_PropertyModifier::GetPropertyName() const
{
	return LOCTEXT("强韧度", "强韧度");
}

ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(UARPG_Toughness_PropertyModifier, ACharacterBase, Toughness);

#undef LOCTEXT_NAMESPACE
