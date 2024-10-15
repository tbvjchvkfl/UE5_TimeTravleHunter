// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/InventoryItem.h"
#include "Object/PickUpItem.h"

// Engine
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UInventoryItem::InitializeInventoryItem(APickUpItem *PickUpItem)
{
	if (PickUpItem)
	{
		ItemImage->SetBrushFromTexture(PickUpItem->GetItemTexture());
		ItemQuantityText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PickUpItem->GetCurrentQuantity())));

		BackGroundSizeBox->SetWidthOverride(PickUpItem->GetMaxSize(PickUpItem->GetItemRotation(), true).X * 100.0f);
		BackGroundSizeBox->SetHeightOverride(PickUpItem->GetMaxSize(PickUpItem->GetItemRotation(), true).Y * 100.0f);
	}
}