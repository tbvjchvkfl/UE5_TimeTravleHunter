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
	InitializedItemData();
}

void UItemPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemPoolComponent::InitializedItemData()
{
	TArray<FItemData*> RowArray;
	ItemDataTable->GetAllRows<FItemData>("", RowArray);
	for (auto ItemDataElem : RowArray)
	{
		PickUpItem = GetWorld()->SpawnActor<APickUpItem>(PickUpItemClass);
		PickUpItem->SetItemData(ItemDataElem);
		PickUpItem->SetActivateItem(false);
		ItemPool.Add(PickUpItem);
	}
	bIsFillItemPool = true;
}

