// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/EquipmentSlot.h"
#include "UI/EquipmentContents.h"
#include "UI/Equipment.h"
#include "Object/PickUpItem.h"

// Engine
#include "Components/Image.h"

void UEquipmentSlot::InitializeEquipmentSlot(UEquipmentContents *EquipContentsWidget, APickUpItem *WeaponItem)
{
	if (EquipContentsWidget)
	{
		EquipContents = EquipContentsWidget;
		ItemInfo = WeaponItem;
		if (EquipContents)
		{
			MainWeaponSlotState = EquipContents->GetWeaponButtonStateMain() && EquipContents->GetEquipSlotMain();
			SubWeaponSlotState = EquipContents->GetWeaponButtonStateSub() && EquipContents->GetEquipSlotSub();
			RangedWeaponSlotState = EquipContents->GetWeaponButtonStateRanged() && EquipContents->GetEquipSlotRanged();
		}
		WeaponImage->SetRenderOpacity(0.0f);
		HoverImage->SetRenderOpacity(0.0f);

		SetItemInfo(ItemInfo);
	}
}

void UEquipmentSlot::SetItemInfo(APickUpItem *Item)
{
	if (Item)
	{
		WeaponImage->SetRenderOpacity(1.0f);
		WeaponImage->SetBrushFromTexture(Item->GetItemTexture());
	}
	else
	{
		WeaponImage->SetRenderOpacity(0.0f);
	}
}

void UEquipmentSlot::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	if (EquipContents)
	{
		MainWeaponSlotState = EquipContents->GetWeaponButtonStateMain() && EquipContents->GetEquipSlotMain();
		SubWeaponSlotState = EquipContents->GetWeaponButtonStateSub() && EquipContents->GetEquipSlotSub();
		RangedWeaponSlotState = EquipContents->GetWeaponButtonStateRanged() && EquipContents->GetEquipSlotRanged();
	}
}

void UEquipmentSlot::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	if (ItemInfo)
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
	if (ItemInfo)
	{
		HoverImage->SetRenderOpacity(0.0f);
		HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (ItemInfo && EquipContents)
		{
			if (MainWeaponSlotState)
			{
				OnAddWidget.Broadcast(EquipContents->GetEquipSlotMain());
			}
			if (SubWeaponSlotState)
			{
				OnAddWidget.Broadcast(EquipContents->GetEquipSlotSub());
			}
			if (RangedWeaponSlotState)
			{
				OnAddWidget.Broadcast(EquipContents->GetEquipSlotRanged());
			}
			EquipContents->OnEquipWeaponWidget.Broadcast(this);
			OnRemoveWidget.Broadcast(this);
			return FReply::Handled();
		}
	}
	return FReply::Unhandled();
}
