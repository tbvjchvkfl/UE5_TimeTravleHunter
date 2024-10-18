// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/InventoryItem.h"
#include "UI/ItemVisibility.h"
#include "UI/DropDown.h"
#include "Object/PickUpItem.h"
#include "Controller/PlayerCharacterController.h"

// Engine
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/GridSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryItem::InitializeInventoryItem(APickUpItem *PickUpItem)
{
	if (PickUpItem)
	{
		for (const auto &ShapeElem : PickUpItem->GetShape(PickUpItem->GetItemRotation()))
		{
			if (ItemVisibility)
			{
				VisibilityWidget = CreateWidget<UItemVisibility>(GetOwningPlayer(), ItemVisibility);
				if (VisibilityWidget)
				{
					VisibilityWidget->VisibilityWidgetInit(ItemGridSize);
					VisibilityContainer.Add(VisibilityWidget);
					VisibilityWidget->OnMouseButtonPressed.AddUObject(this, &UInventoryItem::MouseButtonDown);
					WidgetCanvas->AddChildToCanvas(VisibilityWidget);
					VisibilityWidget->SetRenderTranslation(FVector2D(ShapeElem.X * ItemGridSize, ShapeElem.Y * ItemGridSize));
				}
			}

		}

		ItemImage->SetBrushFromTexture(PickUpItem->GetItemTexture());
		ItemQuantityText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PickUpItem->GetCurrentQuantity())));

		BackGroundSizeBox->SetWidthOverride(PickUpItem->GetMaxSize(PickUpItem->GetItemRotation(), true).X * ItemGridSize);
		BackGroundSizeBox->SetHeightOverride(PickUpItem->GetMaxSize(PickUpItem->GetItemRotation(), true).Y * ItemGridSize);
	}
}

void UInventoryItem::MouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (DropDown)
		{
			//RemoveFromViewport();
			DropDownWidget = CreateWidget<UDropDown>(GetOwningPlayer(), DropDown);
			if (DropDownWidget)
			{
				DropDownWidget->AddToViewport();
				FVector2D MousePosition;
				GetMousePosition(MousePosition);
				DropDownWidget->SetRenderTranslation(MousePosition);
				DropDownWidget->OnDropItem.AddUObject(this, &UInventoryItem::DropItem);
				DropDownWidget->OnRemoveItem.AddUObject(this, &UInventoryItem::RemoveItem);
				DropDownWidget->OnMouseEnter.AddUObject(this, &UInventoryItem::HoverItem);
				DropDownWidget->OnMouseLeave.AddUObject(this, &UInventoryItem::UnHoverItem);
			}
		}
		
	}
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		StartMovingItem();
	}
}

bool UInventoryItem::GetMousePosition(FVector2D &MousePos) const
{
	auto PlayerController = Cast<APlayerCharacterController>(GetOwningPlayer());
	if (PlayerController)
	{
		return PlayerController->GetMousePosition(MousePos.X, MousePos.Y);
	}
	return false;
}

void UInventoryItem::DropItem()
{
}

void UInventoryItem::RemoveItem()
{
}

void UInventoryItem::HoverItem()
{
	for (auto VisibilityElem : VisibilityContainer)
	{
		VisibilityElem->ChangeColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f));
	}
}

void UInventoryItem::UnHoverItem()
{
	for (auto VisibilityElem : VisibilityContainer)
	{
		VisibilityElem->ChangeColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

void UInventoryItem::RemoveDropDown()
{
	if (DropDownWidget)
	{
		DropDownWidget->RemoveFromParent();
	}
}

void UInventoryItem::StartMovingItem()
{
	RemoveDropDown();
	OnMoved.Broadcast(this);
	WidgetCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	for (auto VisibilityElem : VisibilityContainer)
	{
		VisibilityElem->StartBlink();
	}
}

void UInventoryItem::StopMovingItem()
{
	WidgetCanvas->SetVisibility(ESlateVisibility::HitTestInvisible);
	for (auto VisibilityElem : VisibilityContainer)
	{
		VisibilityElem->StopBlink();
	}
}

bool UInventoryItem::IsOverlapping(FVector2D CurrentLocation, APickUpItem *Item) const
{
	for (const auto &ShapeElem : Item->GetShape(Item->GetItemRotation()))
	{
		if (ShapeElem + Location == CurrentLocation)
		{
			return true;
		}
	}
	return false;
}

void UInventoryItem::GetCurrentGridLocation(float &LocationX, float &LocationY) const
{
	UGridSlot *GridSlot = Cast<UGridSlot>(this->Slot);
	LocationX = GridSlot->GetColumn();
	LocationY = GridSlot->GetRow();
}

void UInventoryItem::UpdateVisual(APickUpItem *Item, UGridSlot* GridSlot)
{
	ItemImage->SetRenderTransformAngle(Item->GetItemRotation());
	if (Item->GetItemRotation() == 90.0f)
	{
		ItemImage->SetRenderTranslation(FVector2D(ItemGridSize * GridSlot->GetRowSpan(), 0.0f));
	}
	else
	{
		ItemImage->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	}
}
