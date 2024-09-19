// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"
#include "Object/PickUpItem.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CoinInventory = 0;
	Columns = 15;
	Rows = 6;
}

void UInventoryComponent::AddInventory(class APickUpItem *Item)
{
	// Add to InventoryArray
	switch (Item->GetItemType())
	{
		case EItemType::Coin:
		{
			CoinInventory += Item->GetItemAmount();
		}
		break;
		case EItemType::Weapon:
		{
			AddItem(Item);
		}
		break;
		case EItemType::Consumable:
		{
			AddItem(Item);
		}
		break;
		default:
		{
			return;
		}
		break;
	}
	OnInventoryUpdate.Broadcast();
}

TMap<class APickUpItem *, FVector2D> UInventoryComponent::GetAllItems()
{
	TMap<APickUpItem*, FVector2D> Items;

	for (int i = 0; i < ItemInventory.Num(); i++)
	{
		if (ItemInventory[i] != nullptr && !Items.Contains(ItemInventory[i]))
		{
			Items.Add(ItemInventory[i], FVector2D(i % Columns, i / Columns));
		}
	}
	return Items;
}



void UInventoryComponent::AddItem(APickUpItem *Items)
{
	if (Items)
	{
		for (int32 i = 0; i < ItemInventory.Num(); i++)
		{
			if (bIsRoomAvailable(Items, i))
			{
				AddingItem(Items, i);
			}
		}
		for (int32 i = 0; i < ItemInventory.Num(); i++)
		{
			Items->ToggleRotated();
			if (bIsRoomAvailable(Items, i))
			{
				AddingItem(Items, i);
			}
			Items->ToggleRotated();
		}
	}
}

void UInventoryComponent::AddingItem(class APickUpItem *Items, int32 TopLeftIndex)
{
	int32 tileX = TopLeftIndex % Columns;
	int32 tileY = TopLeftIndex / Columns;
	for (int32 n = tileX; n < tileX + Items->GetDimensions().X; n++)
	{
		for (int32 m = tileY; m < tileY + Items->GetDimensions().Y; m++)
		{
			int32 index = n + m * Columns;
			ItemInventory[index] = Items;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Add Success")), true);
	bIsDirty = true;
}

bool UInventoryComponent::bIsRoomAvailable(APickUpItem *Items, int32 TopLeftIndex) const
{
	int32 tileX = TopLeftIndex % Columns;
	int32 tileY = TopLeftIndex / Columns;
	for (int32 n = tileX; n < tileX + Items->GetDimensions().X; n++)
	{
		for (int32 m = tileY; m < tileY + Items->GetDimensions().Y; m++)
		{
			if (n >= 0 && m >= 0 && n < Columns && m < Rows)
			{
				int32 index = n + m * Columns;
				if (!ItemInventory.IsValidIndex(index))
				{
					return false;
				}
				if (ItemInventory[index] != nullptr)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ItemInventory.Init(nullptr, Rows * Columns);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

