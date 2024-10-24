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
		GridWidget->OnDropItem.AddUObject(this, &UInventory::DropItem);
		GridWidget->OnRemoveItem.AddUObject(this, &UInventory::RemoveItem);
		GridWidget->OnAddItem.AddUObject(this, &UInventory::AddItem);
		GridWidget->InventoryGridInitialize();
	}
}

void UInventory::DropItem(FVector2D Pos)
{

}

void UInventory::RemoveItem(FVector2D Pos)
{

}

void UInventory::AddItem(FVector2D Pos, APickUpItem *Item)
{

}