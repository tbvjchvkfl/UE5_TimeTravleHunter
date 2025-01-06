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
		WeaponItemInventory.SetNum(EquipInventorySize);
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
			WeaponItem = CreateWidget<UWeaponItemWidget>(GetOwningPlayer(), WeaponItemWidget);
			if (WeaponItem)
			{
				WeaponItem->InitializeWeaponItem(this, CurrentWeaponWidget, EquipWeaponInventory[i]);
				WeaponItem->OnAddItemWidget.AddUObject(this, &UEquipWeaponWidget::AddEquipItem);
				WeaponItem->OnRemoveItemWidget.AddUObject(this, &UEquipWeaponWidget::RemoveEquipItem);
				WeaponItemInventory[i] = WeaponItem;
				WeaponWrapPanel->AddChildToWrapBox(WeaponItem);
			}
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

void UEquipWeaponWidget::AddEquipItem(APickUpItem *Item, UWeaponItemWidget *WidgetItem)
{
	for (int32 i = 0; i < EquipWeaponInventory.Num(); i++)
	{
		if (!InventoryComponent->GetRoomCheckingInventory()[i])
		{
			OnAddEquipItem.Broadcast(i, Item);

			WeaponItemInventory[i] = WidgetItem;
			EquipWeaponInventory.Add(Item);
			WeaponWrapPanel->AddChildToWrapBox(WidgetItem);
		}
	}
}

void UEquipWeaponWidget::RemoveEquipItem(UWeaponItemWidget *WidgetItem)
{
	for (int32 i = 0; i < WeaponItemInventory.Num(); i++)
	{
		if (WeaponItemInventory[i] == WidgetItem)
		{
			if (InventoryComponent->GetRoomCheckingInventory()[i])
			{
				WeaponWrapPanel->RemoveChild(WeaponItemInventory[i]);
				WeaponItemInventory.RemoveAt(i);
				EquipWeaponInventory.RemoveAt(i);

				OnRemoveWeaponItem.Broadcast(i);
			}
			return;
		}
	}
}

FEventReply UEquipWeaponWidget::OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent &MouseEvent)
{
	return FEventReply();
}

void UEquipWeaponWidget::OnMouseEnter(FGeometry MyGeometry, const FPointerEvent &MouseEvent)
{
	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, FString("MouseEnter"));
}

void UEquipWeaponWidget::OnMouseLeave(const FPointerEvent &MouseEvent)
{
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString("MouseLeave"));
}
