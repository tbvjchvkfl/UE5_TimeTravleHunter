// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/Inventory.h"
#include "UI/InventoryGrid.h"
#include "UI/EquipWeaponWidget.h"
#include "UI/Equipment.h"
#include "UI/ItemViewUI.h"
#include "Character/Player/PlayerCharacter.h"
#include "Object/PickUpItem.h"
#include "Object/PickUpItemDummy.h"
#include "Component/InventoryComponent.h"


// Engine
#include "Blueprint/DragDropOperation.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();

		GridWidget->OnDropItem.AddUObject(this, &UInventory::DropItem);
		GridWidget->OnRemoveItem.AddUObject(this, &UInventory::RemoveItem);
		GridWidget->OnAddItem.AddUObject(this, &UInventory::AddItem);
		GridWidget->OnShowDummyItem.AddUObject(this, &UInventory::DesiredItemMesh);
		GridWidget->InventoryGridInitialize();
		

		EquipmentWidget->InitializeEquipmenWidget();
		EquipmentWidget->OnAddWeaponItem.AddUObject(this, &UInventory::AddWeaponItem);
		EquipmentWidget->OnRemoveWeaponItem.AddUObject(this, &UInventory::RemoveWeaponItem);
		EquipmentWidget->OnShowWeaponMesh.AddUObject(this, &UInventory::DesiredItemMesh);

		Equipment->InitializeEquipment();
		Equipment->OnAddWeaponItem.AddUObject(this, &UInventory::AddWeaponItem);
		Equipment->OnRemoveWeaponItem.AddUObject(this, &UInventory::RemoveWeaponItem);
	}
	CheckItemDummyActor();

	BTN_Equip->OnClicked.AddDynamic(this, &UInventory::ShowEquipment);
	BTN_Inventory->OnClicked.AddDynamic(this, &UInventory::ShowInventory);
}

void UInventory::CheckItemDummyActor()
{
	if (PickUpItemDummyClass)
	{
		TArray<AActor *>ActorArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PickUpItemDummyClass, ActorArray);
		for (auto ArrayElem : ActorArray)
		{
			if (ArrayElem)
			{
				PickUpItemDummy = Cast<APickUpItemDummy>(ArrayElem);
				break;
			}
		}
	}
}

void UInventory::DesiredItemMesh(APickUpItem *Item)
{
	PickUpItemDummy->ChangingItemMesh(Item);
}

void UInventory::ShowEquipment()
{
	Equipment->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	GridWidget->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentWidget->SetVisibility(ESlateVisibility::Collapsed);
	ItemViewUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventory::ShowInventory()
{
	Equipment->SetVisibility(ESlateVisibility::Collapsed);
	GridWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	EquipmentWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ItemViewUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UInventory::DropItem(FVector2D Pos)
{
	InventoryComponent->DropItem(InventoryComponent->GetItemInventory()[Pos]);
	RemoveItem(Pos);
}

void UInventory::RemoveItem(FVector2D Pos)
{
	InventoryComponent->GetItemInventory().Remove(Pos);
	InventoryComponent->RemoveFromInventory(Pos, false);
}

void UInventory::AddItem(FVector2D Pos, APickUpItem *Item)
{
	InventoryComponent->GetItemInventory().Add(Pos, Item);
	InventoryComponent->AddtoInventory(Pos, Item, false);
}

void UInventory::RemoveWeaponItem(int32 Index)
{
	InventoryComponent->RemoveWeaponInventory(Index);
}

void UInventory::AddWeaponItem(APickUpItem *Item)
{
	InventoryComponent->AddWeaponInventory(Item);
}
