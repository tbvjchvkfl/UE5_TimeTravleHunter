// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/InventoryItem.h"
#include "UI/ItemVisibility.h"
#include "UI/DropDown.h"
#include "Object/PickUpItem.h"
#include "Character/Controller/PlayerCharacterController.h"

// Engine
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/GridSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryItem::InitializeInventoryItem(FVector2D Loc, APickUpItem *PickUpItem)
{
	if (PickUpItem)
	{
		WidgetLocation = Loc;
		PickUpItems = PickUpItem;
		bIsMovable = false;
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
	OnShowMesh.Broadcast(PickUpItems);
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
			DropDownWidget = nullptr;
		}
		else
		{
			if (DropDown)
			{
				DropDownWidget = CreateWidget<UDropDown>(GetOwningPlayer(), DropDown);
				if (DropDownWidget)
				{
					DropDownWidget->AddToViewport();
					FVector2D MousePosition;
					if (GetMousePositionInViewport(MousePosition))
					{
						DropDownWidget->SetRenderTranslation(MousePosition);

						DropDownWidget->OnDropButton.AddUObject(this, &UInventoryItem::DropItem);
						DropDownWidget->OnRemoveButton.AddUObject(this, &UInventoryItem::RemoveItem);
					}
				}
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
	bIsMovable = true;
	OnMoved.Broadcast(this);
}

void UInventoryItem::StopMovingItem()
{
	WidgetCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	bIsMovable = false;
	for (const auto& VisibilityElem : VisibilityContainer)
	{
		VisibilityElem->StopBlink();
	}
}

void UInventoryItem::ModifyQuantity(int32 NewQuantity)
{
	if (PickUpItems->SetCurrentQuantity(NewQuantity) > 0)
	{
		ItemQuantityText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PickUpItems->GetCurrentQuantity())));
		OnAdded.Broadcast(this);
	}
	else
	{
		RemoveItem();
	}
}

bool UInventoryItem::IsOverlapping(FVector2D CurrentLocation) const
{
	for (const auto &ShapeElem : PickUpItems->GetShape(PickUpItems->GetItemRotation()))
	{
		if (ShapeElem + WidgetLocation == CurrentLocation)
		{
			return true;
		}
	}
	return false;
}

bool UInventoryItem::IsFullyStack(int32 &Remaining)
{
	if (PickUpItems->GetMaxQuantity() - PickUpItems->GetCurrentQuantity() == 0)
	{
		Remaining = PickUpItems->GetMaxQuantity() - PickUpItems->GetCurrentQuantity();
		return true;
	}
	else
	{
		Remaining = PickUpItems->GetMaxQuantity() - PickUpItems->GetCurrentQuantity();
		return false;
	}
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

void UInventoryItem::ItemRotate()
{
	if (PickUpItems)
	{
		if (bIsItemRotated)
		{
			PickUpItems->SetItemRotation(ItemImage->GetRenderTransformAngle() - 90.0f);
			bIsItemRotated = false;
		}
		else
		{
			PickUpItems->SetItemRotation(ItemImage->GetRenderTransformAngle() + 90.0f);
			bIsItemRotated = true;
		}
		UpdateVisual(PickUpItems);
	}
}