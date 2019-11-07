// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PropCoreBase.h"
#include "ARPG_PropBase.h"
#include "ARPG_PropertyOperator.h"
#include "CharacterBase.h"
#include "ARPG_CharacterStateComponent.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

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
	for (const FGeneralPropOperatorConfig& Config : EffectProps)
	{
		if (Config.OperatorProperty)
		{
			Config.OperatorProperty.GetDefaultObject()->ApplyValue(Config.Operand, ItemOwner, Config.Value, FARPG_PropertyChangeContext(this, ItemOwner));
		}
	}
	for (const TSubclassOf<UARPG_CharacterState_BuffBase>& BuffType : Buffs)
	{
		ItemOwner->CharacterStateComponent->ApplyBuffByType(BuffType);
	}
	Super::UseItem(ItemOwner, UseItemInput);
}

#undef LOCTEXT_NAMESPACE
