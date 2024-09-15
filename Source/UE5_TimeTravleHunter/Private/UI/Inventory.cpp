// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/InventoryComponent.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();
		if (InventoryComponent)
		{
			InventoryComponent->OnInventoryUpdate.AddUObject(this, &UInventory::UpdateInventory);
		}
	}
}

void UInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

bool UInventory::NativeOnDrop(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
	return false;
}

void UInventory::UpdateInventory()
{
}
