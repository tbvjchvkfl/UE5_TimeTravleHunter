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
	// Add to InventoryArray
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

//TMap<class APickUpItem *, FVector2D> UInventoryComponent::GetAllItems()
//{
//	TMap<APickUpItem*, FVector2D> Items;
//
//	for (int i = 0; i < ItemInventory.Num(); i++)
//	{
//		if (ItemInventory[i] != nullptr && !Items.Contains(ItemInventory[i]))
//		{
//			Items.Add(ItemInventory[i], FVector2D(i % Columns, i / Columns));
//		}
//	}
//	return Items;
//}

void UInventoryComponent::AddItem(APickUpItem *Items)
{
	if (Items)
	{
		if (bIsRoomAvailable(Items->GetShape(0.0f)))
		{
			
		}
	}
	/*if (Items)
	{
		for (int32 i = 0; i < ItemInventory.Num(); i++)
		{
			if (bIsRoomAvailable(Items, i))
			{
				AddingItem(Items, i);
			}
		}
	}*/
}

//bool UInventoryComponent::CheckRotationFits(AMultiSlotItem *ItemReference, float Rotation)
//{
//	if (WhereShapeFits(ItemReference->GetShape(Rotation)).Key == true)
//	{
//		AddToInventory(WhereShapeFits(ItemReference->GetShape(Rotation)).Value, ItemReference, true);
//		return true;
//	}
//	return false;
//
//}

bool UInventoryComponent::bIsRoomAvailable(TArray<FVector2D> Shape) const
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
					return true;
				}
			}
		}
	}
	return false;
}

bool UInventoryComponent::bIsItemStackable(APickUpItem *Item) const
{
	TArray<FVector2D> AllKeys;
	ItemInventory.GetKeys(AllKeys);
	for (int32 i = 0; i < AllKeys.Num(); i++)
	{
		if (ItemInventory[AllKeys[i]]->GetItemName() == Item->GetItemName())
		{
			if (ItemInventory[AllKeys[i]]->GetMaxQuantity() > Item->GetCurrentQuantity())
			{
				return true;
			}
		}
	}
	return false;
}

void UInventoryComponent::AddingItem(class APickUpItem *Items, int32 TopLeftIndex)
{

}


