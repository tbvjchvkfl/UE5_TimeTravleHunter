// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"
#include "Object/PickUpItem.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CoinInventory = 0;
	InventorySize = 35;
	InventoryWidth = 10;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeInventory();
	InitilaizeEquipInventory();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::InitializeInventory()
{
	ItemInventory.Empty();
	InventoryState.Empty();
	if (InventoryState.IsEmpty())
	{
		int32 Column = 0;
		int32 Row = 0;
		for (int32 i = 0; i < InventorySize; i++)
		{
			InventoryState.Add(FVector2D(Column, Row), false);
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

void UInventoryComponent::InitilaizeEquipInventory()
{
	EquipInventory.Empty();
	if (EquipInventory.IsEmpty())
	{
		for (int32 i = 0; i < EquipInventorySize; i++)
		{
			EquipInventory.Add(nullptr);
		}
	}
}

void UInventoryComponent::AddtoInventory(FVector2D ItemPosition, APickUpItem *Item, bool ModifyState)
{
	ItemInventory.Add(ItemPosition, Item);
	if (ModifyState)
	{
		for (const FVector2D &ShapeElem : Item->GetShape(0.0f))
		{
			InventoryState.Add(ShapeElem + ItemPosition, true);
		}
	}
}

void UInventoryComponent::RemoveFromInventory(FVector2D ItemPosition, bool ModifyState)
{
	ItemInventory.Remove(ItemPosition);
	if (ModifyState)
	{
		for (const FVector2D &ShapeElem : ItemInventory[ItemPosition]->GetShape(0.0f))
		{
			InventoryState.Add(ShapeElem + ItemPosition, false);
		}
	}
}

void UInventoryComponent::DropItem(APickUpItem *Item)
{
	FVector Start{ GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 600.0f) };
	FVector End{ Start + FVector(0.0f, 0.0f, -500.0f) };
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, QueryParams))
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Owner = GetOwner();
		SpawnParameter.Instigator = GetOwner()->GetInstigator();

		auto DropItem = GetWorld()->SpawnActor<APickUpItem>(Item->GetClass(), Hit.ImpactPoint, GetOwner()->GetActorRotation(), SpawnParameter);

		if (DropItem)
		{
			DropItem = Item;
		}
	}
}

void UInventoryComponent::CheckItem(class APickUpItem *Item)
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
			AddWeapon(Item);
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
	TArray<FVector2D> AllKeys;
	ItemInventory.GetKeys(AllKeys);
	for (int32 i = 0; i < AllKeys.Num(); i++)
	{
		if (Items && ItemInventory[AllKeys[i]])
		{
			if (ItemInventory[AllKeys[i]]->GetItemNumber() == Items->GetItemNumber())
			{
				if (ItemInventory[AllKeys[i]]->GetMaxQuantity() > Items->GetCurrentQuantity())
				{
					int32 ItemQuantityReference = ItemInventory[AllKeys[i]]->GetCurrentQuantity() + Items->GetCurrentQuantity();

					int32 RemainQuantity = ItemInventory[AllKeys[i]]->GetMaxQuantity() - ItemInventory[AllKeys[i]]->GetCurrentQuantity();

					ItemInventory[AllKeys[i]]->SetCurrentQuantity(ItemQuantityReference);

					AddtoInventory(AllKeys[i], ItemInventory[AllKeys[i]], false);

					Items->SetCurrentQuantity(Items->GetCurrentQuantity() - RemainQuantity);
					if (Items->GetCurrentQuantity() <= 0)
					{
						return;
					}
				}
			}
		}
	}
	if (Items && bIsRoomAvailable(Items->GetShape(0.0f), ItemPos))
	{
		AddtoInventory(ItemPos, Items, true);
		return;
	}
	if (Items && bIsRoomAvailable(Items->GetShape(90.0f), ItemPos))
	{
		AddtoInventory(ItemPos, Items, true);
		return;
	}
}

void UInventoryComponent::AddWeapon(APickUpItem* Item)
{
	for (auto InventoryElem : EquipInventory)
	{
		if (!InventoryElem)
		{
			InventoryElem = Item->WeaponItemInfo;
			return;
		}
	}
}

bool UInventoryComponent::bIsRoomAvailable(TArray<FVector2D> Shape, FVector2D &ItemPosition) const
{
	bool bIsFaild = false;
	TArray<FVector2D> AllKeys;
	InventoryState.GetKeys(AllKeys);
	for (int32 i = 0; i < AllKeys.Num(); i++)
	{
		if (InventoryState.Contains(AllKeys[i]))
		{
			if (InventoryState[AllKeys[i]] == false)
			{
				for (const auto &ItemShapeElem : Shape)
				{
					if (!InventoryState.Contains(FVector2D(AllKeys[i].X + ItemShapeElem.X, AllKeys[i].Y + ItemShapeElem.Y)))
					{
						bIsFaild = true;
						break;
					}
					if (InventoryState[FVector2D(AllKeys[i].X + ItemShapeElem.X, AllKeys[i].Y + ItemShapeElem.Y)] == true)
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