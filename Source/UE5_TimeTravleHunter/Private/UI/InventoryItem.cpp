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
		PickUpItems = PickUpItem;
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

		UpdateVisual(PickUpItem);

		BackGroundSizeBox->SetWidthOverride(PickUpItem->GetMaxSize(PickUpItem->GetItemRotation(), true).X * ItemGridSize);
		BackGroundSizeBox->SetHeightOverride(PickUpItem->GetMaxSize(PickUpItem->GetItemRotation(), true).Y * ItemGridSize);
	}
}

void UInventoryItem::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	HoverItem();
}

void UInventoryItem::NativeOnMouseLeave(const FPointerEvent &InMouseEvent)
{
	UnHoverItem();
}

void UInventoryItem::MouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (DropDownWidget && DropDownWidget->IsValidLowLevel())
		{
			DropDownWidget->RemoveFromParent();
			DropDown = nullptr;
		}
		if(DropDown)
		{
			DropDownWidget = CreateWidget<UDropDown>(GetOwningPlayer(), DropDown);
			if (DropDownWidget)
			{
				DropDownWidget->AddToViewport();
				FVector2D MousePosition;
				GetMousePositionInViewport(MousePosition);
				DropDownWidget->SetRenderTranslation(MousePosition);

				DropDownWidget->OnDropButton.AddUObject(this, &UInventoryItem::DropItem);
				DropDownWidget->OnRemoveButton.AddUObject(this, &UInventoryItem::RemoveItem);
			}
		}
	}
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		StartMovingItem();
	}
}

bool UInventoryItem::GetMousePositionInViewport(FVector2D &MousePos) const
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
	OnDropped.Broadcast(this);
	RemoveDropDown();
	RemoveFromParent();
}

void UInventoryItem::RemoveItem()
{
	OnRemoved.Broadcast(this);
	RemoveDropDown();
	RemoveFromParent();
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
	if (DropDownWidget->IsValidLowLevel())
	{
		DropDownWidget->RemoveFromParent();
		DropDownWidget = nullptr;
	}
}

void UInventoryItem::StartMovingItem()
{
	WidgetCanvas->SetVisibility(ESlateVisibility::HitTestInvisible);
	for (const auto& VisibilityElem : VisibilityContainer)
	{
		VisibilityElem->StartBlink();
	}
	RemoveDropDown();
	OnMoved.Broadcast(this);
}

void UInventoryItem::StopMovingItem()
{
	WidgetCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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

void UInventoryItem::UpdateVisual(APickUpItem *Item)
{
	if (UGridSlot *GridSlot = Cast<UGridSlot>(this->Slot))
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

		TArray<FVector2D> ShapeContain = Item->GetShape(Item->GetItemRotation());

		FVector2D Corner;

		for (int32 i = 0; i < ShapeContain.Num(); i++)
		{
			VisibilityContainer[i]->SetRenderTranslation(FVector2D(ShapeContain[i].X * ItemGridSize, ShapeContain[i].Y * ItemGridSize));

			if (ShapeContain[i].X >= Corner.X && ShapeContain[i].Y >= Corner.Y)
			{
				Corner = ShapeContain[i];
			}
		}

		
		FVector2D CalculateVector(Corner.X * ItemGridSize, Corner.Y * ItemGridSize);
		ItemQuantityText->SetRenderTranslation(FVector2D(CalculateVector.X + 40.0f, CalculateVector.Y + 60.0f));
	}
}

void UInventoryItem::GetItemRotate()
{
	PickUpItems->SetItemRotation(ItemImage->GetRenderTransformAngle() + 90.0f);
	UpdateVisual(PickUpItems);
}