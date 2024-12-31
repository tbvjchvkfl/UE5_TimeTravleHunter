// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/Inventory.h"
#include "UI/InventoryGrid.h"
#include "UI/EquipWeaponWidget.h"
#include "Character/Player/PlayerCharacter.h"
#include "Object/PickUpItem.h"
#include "Component/InventoryComponent.h"

// Engine
#include "Blueprint/DragDropOperation.h"


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