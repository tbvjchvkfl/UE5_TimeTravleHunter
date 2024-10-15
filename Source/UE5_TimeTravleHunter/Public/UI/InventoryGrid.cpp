// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/InventoryGrid.h"
#include "UI/InventoryItem.h"
#include "UI/InventorySlot.h"
#include "Object/PickUpItem.h"
#include "Component/InventoryComponent.h"
#include "Character/Player/PlayerCharacter.h"

// Engine
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryGrid::InventoryGridInitialize()
{
	RefreshGrid();
	InventoryComponent->OnInventoryUpdate.AddUObject(this, &UInventoryGrid::RefreshGrid);
}

void UInventoryGrid::InitGrid()
{
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();
		InventorySize = InventoryComponent->GetItemInventorySize();
		InventoryWidth = InventoryComponent->GetInventoryWidth();
		WidgetInventory = InventoryComponent->GetItemInventory();
	}
}

void UInventoryGrid::RefreshGrid()
{
	InitGrid();
	SetUpEmptyGrid();
	GridInventory.Empty();
	TArray<FVector2D> AllKeys;
	WidgetInventory.GetKeys(AllKeys);
	for (int32 i = 0; i < AllKeys.Num(); i++)
	{
		if (InventoryItem && WidgetInventory[AllKeys[i]])
		{
			ItemWidget = CreateWidget<UInventoryItem>(GetOwningPlayer(), InventoryItem);
			ItemWidget->InitializeInventoryItem(WidgetInventory[AllKeys[i]]);
			auto GridPanelSlot = InventoryGrid->AddChildToGrid(ItemWidget, AllKeys[i].Y, AllKeys[i].X);
			GridPanelSlot->SetLayer(1);
			GridPanelSlot->SetColumnSpan(WidgetInventory[AllKeys[i]]->GetMaxSize(WidgetInventory[AllKeys[i]]->GetItemRotation(), true).X);
			GridPanelSlot->SetRowSpan(WidgetInventory[AllKeys[i]]->GetMaxSize(WidgetInventory[AllKeys[i]]->GetItemRotation(), true).Y);
		}
	}
}

void UInventoryGrid::SetUpEmptyGrid()
{
	GridState.Empty();
	GridSlot.Empty();
	InventoryGrid->ClearChildren();
	int32 Column = 0;
	int32 Row = 0;
	for (int32 i = 0; i < InventorySize; i++)
	{
		GridState.Add(FVector2D(Column, Row), false);
		if (InventorySlot)
		{
			SlotWidget = CreateWidget<UInventorySlot>(GetOwningPlayer(), InventorySlot);
			GridSlot.Add(SlotWidget);
			InventoryGrid->AddChildToGrid(SlotWidget, Row, Column);
			if (Column == InventoryWidth - 1)
			{
				Column = 0; 
				Row++;
			}
			else
			{
				Column++;
			}
		}
	}
}