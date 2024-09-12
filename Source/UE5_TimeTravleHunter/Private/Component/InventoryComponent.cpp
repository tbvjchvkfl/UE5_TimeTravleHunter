// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"
#include "Object/PickUpItem.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CoinInventory = 0;
}

void UInventoryComponent::AddItemInventory(class APickUpItem *Item)
{
	// Add to InventoryArray
	if (Item->GetItemType() == EItemType::Coin)
	{
		CoinInventory += Item->GetItemAmount();
	}
	else
	{
		ItemInventory.Add(Item);
	}
	OnInventoryUpdate.Broadcast();
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

