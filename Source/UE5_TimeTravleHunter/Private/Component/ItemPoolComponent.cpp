// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Component/ItemPoolComponent.h"
#include "Object/PickUpItem.h"

// Engine
#include "Engine/DataTable.h"

UItemPoolComponent::UItemPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsFillItemPool = false;
}

void UItemPoolComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
}

void UItemPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckItemPoolIsEmpty();
}

void UItemPoolComponent::InitializePool()
{
	for (const auto &e : EditList)
	{
		ItemPool.Add(e.Key);
	}
	TArray<FName> SubArray;
	EditList.GetKeys(SubArray);
	TMap<FName, uint8 *> RowMap = ItemDataTable->GetRowMap();
	TArray<FName> MapArray;
	RowMap.GetKeys(MapArray);
	for (int32 i = 0; i < SubArray.Num(); i++)
	{
		FItemPoolStruct ItemPoolStruct;
		for (int32 j = 0; j < EditList[SubArray[i]]; j++)
		{
			PickUpItem = GetWorld()->SpawnActor<APickUpItem>(PickUpItemClass);
			PickUpItem->SetActivateItem(false);
			ItemPoolStruct.AddToItemList(PickUpItem);
		}
		if (RowMap.Contains(SubArray[i])) 
		{
			if (auto RowData = reinterpret_cast<FItemData *>(RowMap[SubArray[i]]))
			{
				for (APickUpItem *Item : ItemPoolStruct.ItemList)
				{
					Item->SetItemData(SubArray[i], RowData);
				}
			}
		}
		ItemPool[SubArray[i]] = ItemPoolStruct;
	}
	bIsFillItemPool = true;
}

bool UItemPoolComponent::CheckItemPoolIsEmpty()
{
	bool bIsItemPoolEmpty{ false };
	TArray<FName> RowArray;
	ItemPool.GetKeys(RowArray);
	for (int32 i = 0; i < RowArray.Num(); i++)
	{
		if (ItemPool[RowArray[i]].ItemListIsEmpty())
		{
			bIsItemPoolEmpty = true;
			bIsFillItemPool = false;
		}
		else
		{
			bIsItemPoolEmpty = false;
			bIsFillItemPool = true;
		}
	}
	return bIsItemPoolEmpty;
}

void UItemPoolComponent::ReturnItemToPool(APickUpItem *Item)
{
	if (Item)
	{
		TArray<FName> RowArray;
		ItemPool.GetKeys(RowArray);
		for (int32 i = 0; i < RowArray.Num(); i++)
		{
			if (RowArray[i] == Item->GetItemRowName())
			{
				Item->SetActivateItem(false);
				ItemPool[RowArray[i]].AddToItemList(Item);
			}
		}
	}
}

APickUpItem *UItemPoolComponent::UseItemOfPool(FName ItemRowName)
{
	TArray<FName> RowArray;
	ItemPool.GetKeys(RowArray);
	APickUpItem *PickItem{};
	for (int32 i = 0; i < RowArray.Num(); i++)
	{
		if (RowArray[i] == ItemRowName)
		{
			if (!ItemPool[RowArray[i]].ItemListIsEmpty())
			{
				PickItem = ItemPool[RowArray[i]].ItemList[ItemPool[RowArray[i]].ItemList.Num() - 1];
				PickItem->SetActivateItem(true);
				ItemPool[RowArray[i]].RemoveToItemList();
			}
		}
	}
	return PickItem;
}