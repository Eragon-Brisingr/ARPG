// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PropCoreBase.h"
#include "ARPG_PropBase.h"
#include "ARPG_PropertyOperator.h"
#include "CharacterBase.h"
#include "ARPG_CharacterStateComponent.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

void UARPG_CS_PropBuff_Modifier::WhenActived(bool IsFirstInit)
{
	EffectProperty.GetDefaultObject()->PushAdditiveModifier(Owner, FARPG_FloatProperty_ModifyConfig(AdditiveValue, NAME_None, this, Instigator));
	EffectProperty.GetDefaultObject()->PushMultipleModifier(Owner, FARPG_FloatProperty_ModifyConfig(MultipleAdditiveValue, NAME_None, this, Instigator));
}

void UARPG_CS_PropBuff_Modifier::WhenDeactived()
{
	EffectProperty.GetDefaultObject()->PopAdditiveModifier(Owner, FARPG_FloatProperty_ModifyConfig(AdditiveValue, NAME_None, this, Instigator));
	EffectProperty.GetDefaultObject()->PushMultipleModifier(Owner, FARPG_FloatProperty_ModifyConfig(MultipleAdditiveValue, NAME_None, this, Instigator));
}

void UARPG_CS_PropBuff_Modifier::SetAdditiveValueAfterActived(float Value)
{
	EffectProperty.GetDefaultObject()->ChangePushedAdditiveModifier(Owner, FARPG_FloatProperty_ModifyConfig(AdditiveValue, NAME_None, this, Instigator), FARPG_FloatProperty_ModifyConfig(Value, NAME_None, this, Instigator));
	AdditiveValue = Value;
}

void UARPG_CS_PropBuff_Modifier::SetMultipleValueAfterActived(float Value)
{
	EffectProperty.GetDefaultObject()->ChangePushedMultipleModifier(Owner, FARPG_FloatProperty_ModifyConfig(AdditiveValue, NAME_None, this, Instigator), FARPG_FloatProperty_ModifyConfig(Value, NAME_None, this, Instigator));
	MultipleAdditiveValue = Value;
}

void UARPG_CS_PropBuff_Operator::WhenTick(float DeltaTime)
{
	EffectProperty.GetDefaultObject()->AddValue(Owner, TickAdditiveValue * DeltaTime, FARPG_PropertyChangeContext(this, Instigator));
}

FText UARPG_PropCoreBase::GetItemTypeDesc() const
{
	return LOCTEXT("道具", "道具");
}

TSubclassOf<AXD_ItemBase> UARPG_PropCoreBase::GetStaticMeshActor() const
{
	return AARPG_Prop_StaticMesh::StaticClass();
}

TSubclassOf<AXD_ItemBase> UARPG_PropCoreBase::GetSkeletalMeshActor() const
{
	return AARPG_Prop_SkeletalMesh::StaticClass();
}

void UARPG_PropCore_General::UseItem(ACharacterBase* ItemOwner, EUseItemInput UseItemInput)
{
	UARPG_CharacterStateComponent* CharacterStateComponent = ItemOwner->CharacterStateComponent;
	for (const TSubclassOf<UARPG_CharacterState_BuffBase>& BuffType : Buffs)
	{
		CharacterStateComponent->TryAddBuffByType(BuffType, FARPG_PropertyChangeContext(this, ItemOwner));
	}
	for (const FPropBuffModifierConfig& ModifierConfig : PropBuff.EffectModifiers)
	{
		if (TSubclassOf<UARPG_GameplayFloatPropertyModifierBase> ModifyProperty = ModifierConfig.ModifyProperty)
		{
			int32 FindIdx = CharacterStateComponent->ActivedBuffes.IndexOfByPredicate([&](UARPG_CharacterState_BuffBase* E)
				{
					if (UARPG_CS_PropBuff_Modifier* PropBuff_Modifier = Cast<UARPG_CS_PropBuff_Modifier>(E))
					{
						return PropBuff_Modifier->EffectProperty == ModifyProperty;
					}
					return false;
				});
			if (FindIdx == INDEX_NONE)
			{
				UARPG_CS_PropBuff_Modifier* PropBuff_Modifier = NewObject<UARPG_CS_PropBuff_Modifier>(CharacterStateComponent);
				PropBuff_Modifier->EffectProperty = ModifyProperty;
				PropBuff_Modifier->DurationTime = PropBuff.Duration;
				switch (ModifierConfig.Operand)
				{
				case EARPG_PropertyModifierOperand::Additive:
					PropBuff_Modifier->AdditiveValue = ModifierConfig.Value;
					break;
				case EARPG_PropertyModifierOperand::Multiple:
					PropBuff_Modifier->MultipleAdditiveValue = ModifierConfig.Value;
					break;
				}
				CharacterStateComponent->AddBuffByRef(PropBuff_Modifier, FARPG_PropertyChangeContext(this, ItemOwner));
			}
			else
			{
				UARPG_CS_PropBuff_Modifier* PropBuff_Modifier = CastChecked<UARPG_CS_PropBuff_Modifier>(CharacterStateComponent->ActivedBuffes[FindIdx]);
				switch (PropBuff.GeneralBuffOverlayType)
				{
				case EPropSpecialBuffOverlayType::CanNotOverlap:
					break;
				case EPropSpecialBuffOverlayType::Replace:
				{
					switch (ModifierConfig.Operand)
					{
					case EARPG_PropertyModifierOperand::Additive:
						PropBuff_Modifier->SetAdditiveValueAfterActived(ModifierConfig.Value);
						break;
					case EARPG_PropertyModifierOperand::Multiple:
						PropBuff_Modifier->SetMultipleValueAfterActived(ModifierConfig.Value);
						break;
					}
					break;
				}
				case EPropSpecialBuffOverlayType::MixExtend:
				{
					const float DurationTime = PropBuff_Modifier->DurationTime + PropBuff.Duration;
					switch (ModifierConfig.Operand)
					{
					case EARPG_PropertyModifierOperand::Additive:
						PropBuff_Modifier->SetAdditiveValueAfterActived((PropBuff_Modifier->AdditiveValue * PropBuff_Modifier->DurationTime + ModifierConfig.Value * PropBuff.Duration) / DurationTime);
						break;
					case EARPG_PropertyModifierOperand::Multiple:
						PropBuff_Modifier->SetMultipleValueAfterActived((PropBuff_Modifier->MultipleAdditiveValue * PropBuff_Modifier->DurationTime + ModifierConfig.Value * PropBuff.Duration) / DurationTime);
						break;
					}
					PropBuff_Modifier->DurationTime = DurationTime;
					break;
				}
				case EPropSpecialBuffOverlayType::MixUseMaxTime:
				{
					const float DurationTime = FMath::Max(PropBuff_Modifier->DurationTime, PropBuff.Duration);
					switch (ModifierConfig.Operand)
					{
					case EARPG_PropertyModifierOperand::Additive:
						PropBuff_Modifier->SetAdditiveValueAfterActived((PropBuff_Modifier->AdditiveValue * PropBuff_Modifier->DurationTime + ModifierConfig.Value * PropBuff.Duration) / DurationTime);
						break;
					case EARPG_PropertyModifierOperand::Multiple:
						PropBuff_Modifier->SetMultipleValueAfterActived((PropBuff_Modifier->MultipleAdditiveValue * PropBuff_Modifier->DurationTime + ModifierConfig.Value * PropBuff.Duration) / DurationTime);
						break;
					}
					PropBuff_Modifier->DurationTime = DurationTime;
					break;
				}

				}
			}
		}
	}
	for (const FPropBuffOperatorConfig& OperatorConfig : PropBuff.TickOperators)
	{
		if (TSubclassOf<UARPG_GameplayFloatPropertyOperatorBase> OperatorProperty = OperatorConfig.OperatorProperty)
		{
			int32 FindIdx = CharacterStateComponent->ActivedBuffes.IndexOfByPredicate([&](UARPG_CharacterState_BuffBase* E)
				{
					if (UARPG_CS_PropBuff_Operator* PropBuff_Modifier = Cast<UARPG_CS_PropBuff_Operator>(E))
					{
						return PropBuff_Modifier->EffectProperty == OperatorProperty;
					}
					return false;
				});
			if (FindIdx == INDEX_NONE)
			{
				UARPG_CS_PropBuff_Operator* PropBuff_Operator = NewObject<UARPG_CS_PropBuff_Operator>(CharacterStateComponent);
				PropBuff_Operator->EffectProperty = OperatorProperty;
				PropBuff_Operator->DurationTime = PropBuff.Duration;
				switch (OperatorConfig.Operand)
				{
				case EPropBuffOperatorOperand::Additive:
					PropBuff_Operator->TickAdditiveValue = OperatorConfig.Value;
					break;
				}
				CharacterStateComponent->AddBuffByRef(PropBuff_Operator, FARPG_PropertyChangeContext(this, ItemOwner));
			}
			else
			{
				UARPG_CS_PropBuff_Operator* PropBuff_Operator = CastChecked<UARPG_CS_PropBuff_Operator>(CharacterStateComponent->ActivedBuffes[FindIdx]);
				switch (PropBuff.GeneralBuffOverlayType)
				{
				case EPropSpecialBuffOverlayType::CanNotOverlap:
					break;
				case EPropSpecialBuffOverlayType::Replace:
					switch (OperatorConfig.Operand)
					{
					case EPropBuffOperatorOperand::Additive:
						PropBuff_Operator->TickAdditiveValue = OperatorConfig.Value;
						break;
					}
					break;
				case EPropSpecialBuffOverlayType::MixExtend:
				{
					const float DurationTime = PropBuff_Operator->DurationTime + PropBuff.Duration;
					switch (OperatorConfig.Operand)
					{
					case EPropBuffOperatorOperand::Additive:
						PropBuff_Operator->TickAdditiveValue = (PropBuff_Operator->TickAdditiveValue * PropBuff_Operator->DurationTime + OperatorConfig.Value * PropBuff.Duration) / DurationTime;
						break;
					}
					PropBuff_Operator->DurationTime = DurationTime;
					break;
				}
				case EPropSpecialBuffOverlayType::MixUseMaxTime:
				{
					const float DurationTime = FMath::Max(PropBuff_Operator->DurationTime, PropBuff.Duration);
					switch (OperatorConfig.Operand)
					{
					case EPropBuffOperatorOperand::Additive:
						PropBuff_Operator->TickAdditiveValue = (PropBuff_Operator->TickAdditiveValue * PropBuff_Operator->DurationTime + OperatorConfig.Value * PropBuff.Duration) / DurationTime;
						break;
					}
					PropBuff_Operator->DurationTime = DurationTime;
					break;
				}
				}
			}
		}
	}

	for (const FGeneralPropOperatorConfig& Config : EffectProps)
	{
		if (Config.OperatorProperty)
		{
			Config.OperatorProperty.GetDefaultObject()->ApplyValue(Config.Operand, ItemOwner, Config.Value, FARPG_PropertyChangeContext(this, ItemOwner));
		}
	}

	Super::UseItem(ItemOwner, UseItemInput);
}

#undef LOCTEXT_NAMESPACE
