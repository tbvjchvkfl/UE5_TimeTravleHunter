// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/WeaponItemWidget.h"
#include "UI/CurrentWeaponWidget.h"
#include "UI/EquipWeaponWidget.h"
#include "Object/PickUpItem.h"
#include "Character/Armor/WeaponBase.h"

// Engine
#include "Components/Image.h"

void UWeaponItemWidget::InitializeWeaponItem(UEquipWeaponWidget *EquipWidget, APickUpItem *WeaponItem)
{
	if (EquipWidget && WeaponItem)
	{
		EquipWeaponWidget = EquipWidget;
		WeaponInfo = WeaponItem;
		WeaponImage->SetBrushFromTexture(WeaponItem->GetItemTexture());
		HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

void UWeaponItemWidget::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f));
	OnShowWeaponItem.Broadcast(WeaponInfo);
}

void UWeaponItemWidget::NativeOnMouseLeave(const FPointerEvent &InMouseEvent)
{
	HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}

FReply UWeaponItemWidget::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		//if (CurrentWeaponItem->PrevWidget)
		//{
			//OnAddItemWidget.Broadcast(CurrentWeaponItem->PrevWidget);
		//}
		//CurrentWeapwzonItem->OnEquipWeapon.Broadcast(this);
		//OnRemoveItemWidget.Broadcast(this);
	}
	return FReply::Handled();
}
