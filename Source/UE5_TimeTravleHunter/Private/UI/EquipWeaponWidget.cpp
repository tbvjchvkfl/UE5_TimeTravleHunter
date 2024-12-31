// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EquipWeaponWidget.h"
#include "Component/InventoryComponent.h"
#include "Character/Player/PlayerCharacter.h"

void UEquipWeaponWidget::InitializeEquipmenWidget()
{
	RefreshInventory();

	EquipWeaponArray.Empty();
	InventoryComponent->OnInventoryUpdate.AddUObject(this, &UEquipWeaponWidget::RefreshInventory);
}

void UEquipWeaponWidget::RefreshInventory()
{
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();
		EquipWeaponArray = InventoryComponent->GetWeaponInventory();
		EquipInventorySize = InventoryComponent->GetWeaponInventorySize();
	}

	EquipWeaponArray.Empty();
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
