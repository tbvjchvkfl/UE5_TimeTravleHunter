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
	CurrentLocation = FVector2D(-999.0f, -999.0f);
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
				ItemWidget->InitializeInventoryItem(AllKeys[i], WidgetInventory[AllKeys[i]]);

				ItemWidget->OnAdded.AddUObject(this, &UInventoryGrid::ItemAdded);
				ItemWidget->OnRemoved.AddUObject(this, &UInventoryGrid::ItemRemoved);
				ItemWidget->OnMoved.AddUObject(this, &UInventoryGrid::ItemMoved);
				ItemWidget->OnDropped.AddUObject(this, &UInventoryGrid::ItemDropped);

				AddToInventory(ItemWidget);

				auto GridPanelSlot = InventoryGrid->AddChildToGrid(ItemWidget, AllKeys[i].Y, AllKeys[i].X);
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
				SlotWidget->InitializeSlot(Column, Row);
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
	AddToInventory(WidgetItem);
}

void UInventoryGrid::ItemRemoved(UInventoryItem *WidgetItem)
{
	RemoveFromInventory(WidgetItem->WidgetLocation);
	OnRemoveItem.Broadcast(WidgetItem->WidgetLocation);
}

void UInventoryGrid::ItemDropped(UInventoryItem *WidgetItem)
{
	OnDropItem.Broadcast(WidgetItem->WidgetLocation);
	RemoveFromInventory(WidgetItem->WidgetLocation);
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

	RemoveFromInventory(WidgetItem->WidgetLocation);
	MovingItem = WidgetItem;
	if (auto *SlotGrid = Cast<UGridSlot>(MovingItem->Slot))
	{
		SlotGrid->SetLayer(2);
	}
}

void UInventoryGrid::MouseEnter(FVector2D Pos)
{
	if (IsValid(MovingItem))
	{
		if (CurrentLocation == FVector2D(-999.0f, -999.0f))
		{
			CurrentLocation = Pos;
		}
		else
		{
			FVector2D OldLocation = CurrentLocation;
			CurrentLocation = Pos;
			UGridSlot *GridPanelSlot = Cast<UGridSlot>(MovingItem->Slot);
			if (IsValid(GridPanelSlot))
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
	if (IsValid(MovingItem))
	{
		if (CanPlaceItem() == true)
		{
			ItemMovedStop();
		}
	}
}

void UInventoryGrid::RightMouseButtonPressed(FVector2D Pos)
{
	if (IsValid(MovingItem))
	{
		MovingItem->ItemRotate();

		if (UGridSlot *GridtoSlot = Cast<UGridSlot>(MovingItem->Slot))
		{
			GridtoSlot->SetColumn(static_cast<int32>(Pos.X));
			GridtoSlot->SetRow(static_cast<int32>(Pos.Y));
		}
	}
}

bool UInventoryGrid::CanPlaceItem() const
{
	if (MovingItem && IsValid(MovingItem))
	{
		APickUpItem *ItemReference = MovingItem->PickUpItems;
		float ItemLoc_X;
		float ItemLoc_Y;
		MovingItem->GetCurrentGridLocation(ItemLoc_X, ItemLoc_Y);
		FVector2D ItemLocation = FVector2D(ItemLoc_X, ItemLoc_Y);

		TArray<FVector2D> ItemShape = ItemReference->GetShape(ItemReference->GetItemRotation());
		for (auto ShapeElem : ItemShape)
		{
			FVector2D RoundedVector = FVector2D(FMath::RoundToInt(ItemLocation.X + ShapeElem.X), FMath::RoundToInt(ItemLocation.Y + ShapeElem.Y));
			if (!GridState.Contains(RoundedVector))
			{
				return false;
			}
			if (GridState[RoundedVector] == true)
			{
				TArray<FVector2D> AllKeys;
				GridInventory.GetKeys(AllKeys);
				for (auto GridElem : AllKeys)
				{
					if (GridInventory[GridElem]->PickUpItems->GetItemNumber() == ItemReference->GetItemNumber())
					{
						if (GridInventory[GridElem]->IsOverlapping(RoundedVector))
						{
							int32 RemainQuantity;
							if (!GridInventory[GridElem]->IsFullyStack(RemainQuantity))
							{
								RemainQuantity -= ItemReference->GetCurrentQuantity();
								int32 ClampQuantity = FMath::Clamp(ItemReference->GetMaxQuantity() - RemainQuantity, 0, ItemReference->GetMaxQuantity());

								GridInventory[GridElem]->ModifyQuantity(ClampQuantity);

								MovingItem->ModifyQuantity(RemainQuantity * -1);

								if (RemainQuantity >= 0)
								{
									ItemWidget->ModifyQuantity(RemainQuantity * -1);

									GridInventory[GridElem]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
									return false;
								}
							}
						}
					}
					else
					{
						return false;
					}
				}
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

void UInventoryGrid::ItemMovedStop()
{
	if (MovingItem)
	{
		OnRemoveItem.Broadcast(MovingItem->WidgetLocation);
		float Grid_X = 0.0f;
		float Grid_Y = 0.0f;
		MovingItem->GetCurrentGridLocation(Grid_X, Grid_Y);
		MovingItem->WidgetLocation = FVector2D(Grid_X, Grid_Y);
		CurrentLocation = FVector2D(-999.0f, -999.0f);
		MovingItem->StopMovingItem();
		if (UGridSlot *SlotGrid = Cast<UGridSlot>(MovingItem->Slot))
		{
			SlotGrid->SetLayer(1);

			AddToInventory(MovingItem);

			OnAddItem.Broadcast(MovingItem->WidgetLocation, MovingItem->PickUpItems);

			MovingItem = nullptr;
			TArray<FVector2D> AllKeys;
			GridInventory.GetKeys(AllKeys);
			for (auto GridInventoryElem : AllKeys)
			{
				GridInventory[GridInventoryElem]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
}

void UInventoryGrid::AddToInventory(UInventoryItem *WidgetItem)
{
	GridInventory.Add(WidgetItem->WidgetLocation, WidgetItem);
	WidgetInventory.Add(WidgetItem->WidgetLocation, WidgetItem->PickUpItems);
	AddToState(WidgetItem->WidgetLocation, WidgetItem->PickUpItems);
}

void UInventoryGrid::RemoveFromInventory(FVector2D WidgetLoc)
{
	RemoveFromState(WidgetLoc, WidgetInventory[WidgetLoc]);
	WidgetInventory.Remove(WidgetLoc);
	GridInventory.Remove(WidgetLoc);
}

void UInventoryGrid::AddToState(FVector2D WidgetLoc, APickUpItem *Item)
{
	if (Item)
	{
		for (const auto &ItemShapeElem : Item->GetShape(Item->GetItemRotation()))
		{
			GridState.Add(ItemShapeElem + WidgetLoc, true);
		}
	}
}

void UInventoryGrid::RemoveFromState(FVector2D WidgetLoc, APickUpItem *Item)
{
	if (Item)
	{
		for (const auto &ItemShapeElem : Item->GetShape(Item->GetItemRotation()))
		{
			GridState.Add(ItemShapeElem + WidgetLoc, false);
		}
	}
}
