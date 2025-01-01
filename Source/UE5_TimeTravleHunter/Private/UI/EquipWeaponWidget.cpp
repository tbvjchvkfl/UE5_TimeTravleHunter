// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/EquipWeaponWidget.h"
#include "UI/EquipmentSlot.h"
#include "UI/WeaponItemWidget.h"
#include "Component/InventoryComponent.h"
#include "Character/Player/PlayerCharacter.h"

// Engine
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

void UEquipWeaponWidget::InitializeEquipmenWidget()
{
	RefreshInventory();
	InventoryComponent->OnInventoryUpdate.AddUObject(this, &UEquipWeaponWidget::RefreshInventory);
}

void UEquipWeaponWidget::RefreshInventory()
{
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();
		EquipWeaponInventory = InventoryComponent->GetWeaponInventory();
		EquipInventorySize = InventoryComponent->GetWeaponInventorySize();
	}
	FillEmptySlot();
	for (auto EquipWeaponElem : EquipWeaponInventory)
	{
		if (EquipWeaponElem)
		{
			WeaponItem = CreateWidget<UWeaponItemWidget>(GetOwningPlayer(), WeaponItemWidget);
			if (WeaponItem)
			{
				WeaponItem->InitializeWeaponItem(EquipWeaponElem);
				EquipWeaponInventory.Add(EquipWeaponElem);
				WeaponItemInventory.Add(WeaponItem);
				WrapPanel->AddChildToWrapBox(WeaponItem);
			}
		}
	}
}

void UEquipWeaponWidget::FillEmptySlot()
{
	WrapPanel->ClearChildren();
	for (int32 i = 0; i < EquipInventorySize; i++)
	{
		EquipmentSlot = CreateWidget<UEquipmentSlot>(GetOwningPlayer(), EquipmentSlotWidget);
		if (EquipmentSlot)
		{
			WrapPanel->AddChildToWrapBox(EquipmentSlot);
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
