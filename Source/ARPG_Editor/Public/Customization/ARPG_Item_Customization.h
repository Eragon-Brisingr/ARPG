// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_Item_Customization.h"

/**
 * 
 */
class ARPG_EDITOR_API FARPG_Item_Customization : public FXD_Item_Customization
{
public:
	FARPG_Item_Customization();

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FARPG_Item_Customization);
	}
};
