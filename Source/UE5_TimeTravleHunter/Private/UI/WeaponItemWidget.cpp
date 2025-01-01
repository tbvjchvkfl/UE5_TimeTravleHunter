// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/WeaponItemWidget.h"
#include "Object/PickUpItem.h"
#include "Character/Armor/WeaponBase.h"

// Engine
#include "Components/Image.h"

void UWeaponItemWidget::InitializeWeaponItem(AWeaponBase *WeaponItem)
{
	if (WeaponItem)
	{
		WeaponImage->SetBrushFromTexture(WeaponItem->WeaponImage);
		
	}
}
