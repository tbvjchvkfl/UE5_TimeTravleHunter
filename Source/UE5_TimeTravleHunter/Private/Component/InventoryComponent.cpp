// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"
#include "Object/PickUpItem.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CoinInventory = 0;
	InventorySize = FIntPoint(10, 4);
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeInventory();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::InitializeInventory()
{
	ItemInventory.Empty();
	if (ItemInventory.IsEmpty())
	{
		for (int32 n = 0; n < InventorySize.Y; n++)
		{
			for (int32 m = 0; m < InventorySize.X; m++)
			{
				ItemInventory.Add(FVector2D(m, n), nullptr);
			}
		}
	}
}

void UInventoryComponent::AddInventory(class APickUpItem *Item)
{
	switch (Item->GetItemType())
	{
		case EItemType::Coin:
		{
			CoinInventory += Item->GetCurrentQuantity();
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

void UInventoryComponent::AddItem(APickUpItem *Items)
{
	FVector2D ItemPos = FVector2D(0, 0);
	for (const auto &ItemInventoryElem : ItemInventory)
	{
		if (Items && ItemInventoryElem.Value)
		{
			if (ItemInventoryElem.Value->GetItemName() == Items->GetItemName())
			{
				if (ItemInventoryElem.Value->GetMaxQuantity() > Items->GetCurrentQuantity())
				{
					int32 ItemQuantityReference = ItemInventoryElem.Value->GetCurrentQuantity() + Items->GetCurrentQuantity();

					ItemInventoryElem.Value->SetCurrentQuantity(ItemQuantityReference);

					ItemInventory.Add(ItemInventoryElem.Key, ItemInventoryElem.Value);

					int32 RemainQuantity = ItemInventoryElem.Value->GetMaxQuantity() - ItemInventoryElem.Value->GetCurrentQuantity();

					Items->SetCurrentQuantity(Items->GetCurrentQuantity() - RemainQuantity);
					if (Items->GetCurrentQuantity() <= 0)
					{
						return;
					}
				}
			}
		}
	}
	if (bIsRoomAvailable(Items->GetShape(0), ItemPos))
	{
		ItemInventory.Add(ItemPos, Items);
		return;
	}
	if (bIsRoomAvailable(Items->GetShape(90.0f), ItemPos))
	{
		ItemInventory.Add(ItemPos, Items);
		return;
	}
}

bool UInventoryComponent::bIsRoomAvailable(TArray<FVector2D> Shape, FVector2D &ItemPosition) const
{
	bool bIsFaild = false;
	TArray<FVector2D> AllKeys;
	ItemInventory.GetKeys(AllKeys);
	for (int32 i = 0; i < AllKeys.Num(); i++)
	{
		if (ItemInventory.Contains(AllKeys[i]))
		{
			if (ItemInventory[AllKeys[i]] == nullptr)
			{
				for (const auto &ItemShapeElem : Shape)
				{
					if (ItemInventory.Contains(FVector2D(AllKeys[i].X + ItemShapeElem.X, AllKeys[i].Y + ItemShapeElem.Y)))
					{
						bIsFaild = true;
						break;
					}
					if (ItemInventory[FVector2D(AllKeys[i].X + ItemShapeElem.X, AllKeys[i].Y + ItemShapeElem.Y)] != nullptr)
					{
						bIsFaild = true;
						break;
					}
				}
				if (bIsFaild)
				{
					bIsFaild = false;
				}
				else
				{
					ItemPosition = AllKeys[i];
					return true;
				}
			}
		}
	}
	ItemPosition = FVector2D(0, 0);
	return false;
}