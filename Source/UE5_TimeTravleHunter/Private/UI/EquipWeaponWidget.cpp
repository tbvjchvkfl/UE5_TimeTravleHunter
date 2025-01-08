// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/EquipWeaponWidget.h"
#include "UI/EquipmentSlot.h"
#include "UI/WeaponItemWidget.h"
#include "UI/CurrentWeaponWidget.h"
#include "Component/InventoryComponent.h"
#include "Character/Player/PlayerCharacter.h"

// Engine
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/PanelSlot.h"


void UEquipWeaponWidget::InitializeEquipmenWidget()
{
	CurrentWeaponWidget->InitializeCurrentWeaponImage();
	RefreshInventory();
	InventoryComponent->OnInventoryUpdate.AddUObject(this, &UEquipWeaponWidget::RefreshInventory);
}

void UEquipWeaponWidget::InitEssentialData()
{
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();
		EquipWeaponInventory = InventoryComponent->GetWeaponInventory();
		EquipInventorySize = InventoryComponent->GetWeaponInventorySize();
		WeaponItemInventory.SetNum(EquipWeaponInventory.Num());
	}
}

void UEquipWeaponWidget::RefreshInventory()
{
	InitEssentialData();
	FillEmptySlot();
	WeaponWrapPanel->ClearChildren();
	for (int32 i = 0; i < EquipWeaponInventory.Num(); i++)
	{
		if (EquipWeaponInventory[i])
		{
			if (!WeaponItemInventory[i])
			{
				WeaponItem = CreateWidget<UWeaponItemWidget>(GetOwningPlayer(), WeaponItemWidget);
				if (WeaponItem)
				{
					WeaponItem->InitializeWeaponItem(this, CurrentWeaponWidget, EquipWeaponInventory[i]);
					WeaponItem->OnAddItemWidget.AddUObject(this, &UEquipWeaponWidget::AddEquipItem);
					WeaponItem->OnRemoveItemWidget.AddUObject(this, &UEquipWeaponWidget::RemoveEquipItem);

					WeaponItemInventory[i] = WeaponItem;
				}
			}
			WeaponWrapPanel->AddChildToWrapBox(WeaponItemInventory[i]);
		}
	}
}

void UEquipWeaponWidget::FillEmptySlot()
{
	SlotWrapPanel->ClearChildren();
	for (int32 i = 0; i < EquipInventorySize; i++)
	{
		EquipmentSlot = CreateWidget<UEquipmentSlot>(GetOwningPlayer(), EquipmentSlotWidget);
		if (EquipmentSlot)
		{
			SlotWrapPanel->AddChildToWrapBox(EquipmentSlot);
		}
	}
}

void UEquipWeaponWidget::AddEquipItem(UWeaponItemWidget *Widget)
{
	WeaponItemInventory.Add(Widget);
	WeaponWrapPanel->AddChildToWrapBox(Widget);
	OnAddWeaponItem.Broadcast(Widget->WeaponInfo);
}

void UEquipWeaponWidget::RemoveEquipItem(UWeaponItemWidget *Widget)
{
	for (int32 i = 0; i < WeaponItemInventory.Num(); i++)
	{
		if (WeaponItemInventory[i] == Widget)
		{
			WeaponItemInventory.RemoveAt(i);
			WeaponWrapPanel->RemoveChildAt(i);
			OnRemoveWeaponItem.Broadcast(i);
		}
	}
}
