// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/Inventory.h"
#include "UI/InventoryGrid.h"
#include "UI/EquipWeaponWidget.h"
#include "UI/CurrentWeaponWidget.h"
#include "Character/Player/PlayerCharacter.h"
#include "Object/PickUpItem.h"
#include "Component/InventoryComponent.h"

// Engine
#include "Blueprint/DragDropOperation.h"
#include "Components/WrapBox.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();

		GridWidget->OnDropItem.AddUObject(this, &UInventory::DropItem);
		GridWidget->OnRemoveItem.AddUObject(this, &UInventory::RemoveItem);
		GridWidget->OnAddItem.AddUObject(this, &UInventory::AddItem);
		GridWidget->InventoryGridInitialize();

		EquipmentWidget->InitializeEquipmenWidget();
		EquipmentWidget->OnAddWeaponItem.AddUObject(this, &UInventory::AddWeaponItem);
		EquipmentWidget->OnRemoveWeaponItem.AddUObject(this, &UInventory::RemoveWeaponItem);
	}
}

void UInventory::DropItem(FVector2D Pos)
{
	InventoryComponent->DropItem(InventoryComponent->GetItemInventory()[Pos]);
	RemoveItem(Pos);
}

void UInventory::RemoveItem(FVector2D Pos)
{
	InventoryComponent->GetItemInventory().Remove(Pos);
	InventoryComponent->RemoveFromInventory(Pos, false);
}

void UInventory::AddItem(FVector2D Pos, APickUpItem *Item)
{
	InventoryComponent->GetItemInventory().Add(Pos, Item);
	InventoryComponent->AddtoInventory(Pos, Item, false);
}

void UInventory::RemoveWeaponItem(int32 Index)
{
	InventoryComponent->RemoveWeaponInventory(Index);
}

void UInventory::AddWeaponItem(APickUpItem *Item)
{
	InventoryComponent->AddWeaponInventory(Item);
}
