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
			if (ItemWidget)
			{
				ItemWidget->InitializeInventoryItem(WidgetInventory[AllKeys[i]]);
				ItemWidget->OnAdded.AddUObject(this, &UInventoryGrid::ItemAdded);
				ItemWidget->OnRemoved.AddUObject(this, &UInventoryGrid::ItemRemoved);
				ItemWidget->OnMoved.AddUObject(this, &UInventoryGrid::ItemMoved);
				ItemWidget->OnDropped.AddUObject(this, &UInventoryGrid::ItemDropped);
				auto GridPanelSlot = InventoryGrid->AddChildToGrid(ItemWidget, AllKeys[i].Y, AllKeys[i].X);
				ItemWidget->UpdateVisual(WidgetInventory[AllKeys[i]], GridPanelSlot);
				GridPanelSlot->SetLayer(1);
				GridPanelSlot->SetColumnSpan(WidgetInventory[AllKeys[i]]->GetMaxSize(WidgetInventory[AllKeys[i]]->GetItemRotation(), true).X);
				GridPanelSlot->SetRowSpan(WidgetInventory[AllKeys[i]]->GetMaxSize(WidgetInventory[AllKeys[i]]->GetItemRotation(), true).Y);
			}
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
			if (SlotWidget)
			{
				GridSlot.Add(SlotWidget);
				InventoryGrid->AddChildToGrid(SlotWidget, Row, Column);
				SlotWidget->OnLeftMouseButtonPressed.AddUObject(this, &UInventoryGrid::LeftMouseButtonPressed);
				SlotWidget->OnRightMouseButtonPressed.AddUObject(this, &UInventoryGrid::RightMouseButtonPressed);
				SlotWidget->OnMouseButtonEnter.AddUObject(this, &UInventoryGrid::MouseEnter);
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
}

void UInventoryGrid::ItemAdded(UInventoryItem *WidgetItem)
{
}

void UInventoryGrid::ItemRemoved(UInventoryItem *WidgetItem)
{
}

void UInventoryGrid::ItemDropped(UInventoryItem *WidgetItem)
{
}

void UInventoryGrid::ItemMoved(UInventoryItem *WidgetItem)
{
	TArray<FVector2D> AllKeys;
	GridInventory.GetKeys(AllKeys);
	for (auto &Key : AllKeys)
	{
		if (GridInventory[Key])
		{
			GridInventory[Key]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	ItemWidget = WidgetItem;
	if (auto *SlotGrid = Cast<UGridSlot>(ItemWidget))
	{
		SlotGrid->SetLayer(2);
	}
}

void UInventoryGrid::MouseEnter(FVector2D Pos)
{
	if (ItemWidget)
	{
		if (CurrentLocation == FVector2D(-999.0f, -999.0f))
		{
			CurrentLocation = Pos;
		}
		else
		{
			FVector2D OldLocation = CurrentLocation;
			CurrentLocation = Pos;
			if (UGridSlot *GridPanelSlot = Cast<UGridSlot>(ItemWidget))
			{
				FVector2D GettingSlotSize(GridPanelSlot->GetColumn(), GridPanelSlot->GetRow());
				FVector2D CalculateLoc(CurrentLocation.X - OldLocation.X, CurrentLocation.Y - OldLocation.Y);

				FVector2D AddingVector(GettingSlotSize.X + CalculateLoc.X, GettingSlotSize.Y + CalculateLoc.Y);

				int32 ColSize = static_cast<int32>(AddingVector.X);
				int32 RowSize = static_cast<int32>(AddingVector.Y);

				GridPanelSlot->SetColumn(ColSize);
				GridPanelSlot->SetRow(RowSize);
			}
		}
	}
}

void UInventoryGrid::LeftMouseButtonPressed(FVector2D Pos)
{
	if (ItemWidget) 
	{
		if (CanPlaceItem())
		{
			ItemStoppedMove();
		}
	}
}

void UInventoryGrid::RightMouseButtonPressed(FVector2D Pos)
{
	if (ItemWidget)
	{
		if (UGridSlot *GridtoSlot = Cast<UGridSlot>(ItemWidget))
		{
			GridtoSlot->SetColumn(static_cast<int32>(Pos.X));
			GridtoSlot->SetRow(static_cast<int32>(Pos.Y));
		}
	}
}

bool UInventoryGrid::CanPlaceItem() const
{
	return false;
}

bool UInventoryGrid::ItemStoppedMove()
{
	return false;
}
