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

				WeaponItemInventory[i] = WeaponItem;
				if (WeaponItemInventory[i]->GetVisibility() != ESlateVisibility::Collapsed)
				{
					WeaponWrapPanel->AddChildToWrapBox(WeaponItem);
				}
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

void UEquipWeaponWidget::AddEquipItem(UWeaponItemWidget *Widget)
{
	GEngine->AddOnScreenDebugMessage(40, 10, FColor::Green, FString("Visibility On"));
	for (int32 i = 0; i < WeaponItemInventory.Num(); i++)
	{
		if (WeaponItemInventory[i] == Widget)
		{
			WeaponItemInventory[i]->SetVisibility(ESlateVisibility::Visible);
		}
	}
}