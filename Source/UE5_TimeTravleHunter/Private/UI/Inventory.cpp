// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/Inventory.h"
#include "UI/InventoryGrid.h"
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
		GridWidget->InventoryComponent = Player->GetItemInventory();
		GridWidget->InventorySize = Player->GetItemInventory()->GetItemInventorySize();
		GridWidget->InventoryWidth = Player->GetItemInventory()->GetInventoryWidth();
		GridWidget->WidgetInventory = Player->GetItemInventory()->GetItemInventory();
		GridWidget->InventoryGridInitialize();
	}
}

bool UInventory::NativeOnDrop(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
	//auto *Item = Cast<APickUpItem>(InOperation->Payload);
	return false;
}