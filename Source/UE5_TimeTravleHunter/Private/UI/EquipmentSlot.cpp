// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/EquipmentSlot.h"
#include "UI/EquipmentContents.h"
#include "UI/Equipment.h"
#include "Object/PickUpItem.h"

// Engine
#include "Components/Image.h"

void UEquipmentSlot::InitializeEquipmentSlot(UEquipmentContents *EquipContentsWidget)
{
	EquipContents = EquipContentsWidget;
	if (EquipContents)
	{
		EquipmentWidget = EquipContents->GetEquipment();
		if (EquipmentWidget)
		{
			MainWeaponSlotState = EquipmentWidget->GetbIsButtonActiveMain() && EquipmentWidget->GetMainWeaponSlot();
			SubWeaponSlotState = EquipmentWidget->GetbIsButtonActiveSub() && EquipmentWidget->GetSubWeaponSlot();
			RangedWeaponSlotState = EquipmentWidget->GetbIsButtonActiveRanged() && EquipmentWidget->GetRangedWeaponSlot();
		}
	}
	WeaponImage->SetRenderOpacity(0.0f);
	HoverImage->SetRenderOpacity(0.0f);
}

void UEquipmentSlot::GetItemInfo(APickUpItem *Item)
{
	WeaponItem = Item;
	if (WeaponItem)
	{
		WeaponImage->SetRenderOpacity(1.0f);
		WeaponImage->SetBrushFromTexture(Item->GetItemTexture());
	}
}

void UEquipmentSlot::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	if (WeaponItem)
	{
		HoverImage->SetRenderOpacity(1.0f);
		HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f));
	}
	else
	{
		HoverImage->SetRenderOpacity(0.0f);
	}
}

void UEquipmentSlot::NativeOnMouseLeave(const FPointerEvent &InMouseEvent)
{
	if (WeaponItem)
	{
		HoverImage->SetRenderOpacity(0.0f);
		HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (WeaponItem && EquipContents)
		{
			if (MainWeaponSlotState)
			{
				EquipContents->OnAddWeaponWidget.Broadcast(EquipmentWidget->GetMainWeaponSlot());
			}
			if (SubWeaponSlotState)
			{
				EquipContents->OnAddWeaponWidget.Broadcast(EquipmentWidget->GetSubWeaponSlot());
			}
			if (RangedWeaponSlotState)
			{
				EquipContents->OnAddWeaponWidget.Broadcast(EquipmentWidget->GetRangedWeaponSlot());
			}
			EquipContents->OnEquipWeaponWidget.Broadcast(this);
			EquipContents->OnRemoveWeaponWidget.Broadcast(this);
			return FReply::Handled();
		}
	}
	return FReply::Unhandled();
}
