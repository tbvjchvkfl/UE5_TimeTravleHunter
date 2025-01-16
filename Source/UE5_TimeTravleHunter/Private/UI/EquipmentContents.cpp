// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/EquipmentContents.h"
#include "UI/EquipmentSlot.h"
#include "UI/Equipment.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/InventoryComponent.h"

// Engine
#include "Components/ScrollBox.h"

void UEquipmentContents::InitializeEquipmentContents(UEquipment* EquipmentWidget)
{
	RefreshEquipmentSlot();
	EquipWidget = EquipmentWidget;
	InventoryComponent->OnInventoryUpdate.AddUObject(this, &UEquipmentContents::RefreshEquipmentSlot);
	OnEquipWeaponWidget.AddUObject(this, &UEquipmentContents::EquipWeaponWidget);
	OnRemoveWeaponWidget.AddUObject(this, &UEquipmentContents::RemoveWeaponWidget);
	OnAddWeaponWidget.AddUObject(this, &UEquipmentContents::AddWeaponWidget);
}

void UEquipmentContents::InitEssentialData()
{
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();
		ItemList = InventoryComponent->GetWeaponInventory();
		ListSize = InventoryComponent->GetWeaponInventorySize();
	}
}

void UEquipmentContents::RefreshEquipmentSlot()
{
	InitEssentialData();
	FillEmptySlot();
	for (int32 i = 0; i < ItemList.Num(); i++)
	{
		if (ItemList[i])
		{
			if (WidgetList[i] && !WidgetList[i]->GetWeaponItem())
			{
				WidgetList[i]->GetItemInfo(ItemList[i]);
			}
		}
	}
}

void UEquipmentContents::FillEmptySlot()
{
	WidgetList.Empty();
	ContentsBox->ClearChildren();
	for (int32 i = 0; i < ListSize; i++)
	{
		if (EquipmentSlotWidget)
		{
			EquipmentSlot = CreateWidget<UEquipmentSlot>(GetOwningPlayer(), EquipmentSlotWidget);

			if (EquipmentSlot)
			{
				EquipmentSlot->InitializeEquipmentSlot(this);
				WidgetList.Add(EquipmentSlot);
				ContentsBox->AddChild(EquipmentSlot);
			}
		}
	}
}

void UEquipmentContents::EquipWeaponWidget(UEquipmentSlot *SlotWidget)
{
	EquipWidget->OnEquipWeapon.Broadcast(SlotWidget);
}

void UEquipmentContents::RemoveWeaponWidget(UEquipmentSlot *SlotWidget)
{
	for (int32 i = 0; i < WidgetList.Num(); i++)
	{
		if (WidgetList[i] == SlotWidget)
		{
			EquipWidget->OnRemoveWeaponItem.Broadcast(i);
			WidgetList.RemoveAt(i);
			ContentsBox->RemoveChildAt(i);
			return;
		}
	}
}

void UEquipmentContents::AddWeaponWidget(UEquipmentSlot *SlotWidget)
{
	for (int32 i = 0; i < WidgetList.Num(); i++)
	{
		if (!WidgetList[i]->GetWeaponItem())
		{
			WidgetList[i] = SlotWidget;
			ContentsBox->AddChild(SlotWidget);
			EquipWidget->OnAddWeaponItem.Broadcast(SlotWidget->GetWeaponItem());
			return;
		}
	}
}
