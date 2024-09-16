// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/Inventory.h"
#include "UI/InventoryGird.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/InventoryComponent.h"

// Engine

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();
		TileSize = 100.0f;
		if (InventoryComponent)
		{
			InventoryGrid->GridInitialize(InventoryComponent, TileSize);
			InventoryComponent->OnInventoryUpdate.AddUObject(this, &UInventory::UpdateInventory);
		}
	}
}

bool UInventory::NativeOnDrop(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
	return false;
}

void UInventory::UpdateInventory()
{
}
