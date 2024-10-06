// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/InventoryGrid.h"
#include "UI/InventoryItem.h"
#include "Object/PickUpItem.h"
#include "Component/InventoryComponent.h"
#include "Character/Player/PlayerCharacter.h"

// Engine
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryGrid::GridInitialize(class UInventoryComponent *InventoryComp, float Tilesize)
{
	GridTileSize = Tilesize;
	GridBorder->SetDesiredSizeScale(FVector2D(InventoryComp->GetColumns() * GridTileSize, InventoryComp->GetRows() * GridTileSize));
	if (auto CanvasSlot = Cast<UCanvasPanelSlot>(GridBorder->Slot))
	{
		CanvasSlot->SetSize(FVector2D(InventoryComp->GetColumns() * GridTileSize, InventoryComp->GetRows() * GridTileSize));
	}
	RefreshGridInventory();
	CreateLine(InventoryComp, GridTileSize);
	//InventoryComp->OnInventoryUpdate.AddDynamic(this, &UInventoryGird::RefreshGridInventory);
	InventoryComp->OnInventoryUpdate.AddUObject(this, &UInventoryGrid::RefreshGridInventory);
}

void UInventoryGrid::CreateLine(class UInventoryComponent *InventoryComp, float Tilesize)
{
	for (int i = 0; i <= InventoryComp->GetColumns(); i++)
	{
		LINES.Add(FInventoryLine(FVector2D(i * Tilesize, 0), FVector2D(i * Tilesize, InventoryComp->GetRows() * Tilesize)));
	}
	for (int i = 0; i <= InventoryComp->GetRows(); i++)
	{
		LINES.Add(FInventoryLine(FVector2D(0, i * Tilesize), FVector2D(InventoryComp->GetColumns() * Tilesize, i * Tilesize)));
	}
}

void UInventoryGrid::RefreshGridInventory()
{
	GridCanvas->ClearChildren();
	auto *Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		ReferenceInventory = Player->GetItemInventory();
		if (ReferenceInventory)
		{
			TMap<class APickUpItem *, FVector2D> ItemMaps = ReferenceInventory->GetAllItems();
			for (const auto &Item : ItemMaps)
			{
				ItemWidget = CreateWidget<UInventoryItem>(GetOwningPlayer(), InventoryItem);
				//ItemWidget->OnRemoveItem.AddDynamic(this, &UInventoryGird::RemoveItemInWidget);
				ItemWidget->InitializeInventoryItem(Item.Key);
				UPanelSlot *PanelSlot = GridCanvas->AddChild(ItemWidget);
				UCanvasPanelSlot *CanvasPanelSlot = Cast<UCanvasPanelSlot>(PanelSlot);
				CanvasPanelSlot->SetAutoSize(true);
				CanvasPanelSlot->SetPosition(FVector2D(Item.Value.X * GridTileSize, Item.Value.Y * GridTileSize));
			}
		}
	}
}

void UInventoryGrid::RemoveItemInWidget(class APickUpItem *PickUpItem)
{
	ReferenceInventory->RemoveItem(PickUpItem);
	RefreshGridInventory();
}

int32 UInventoryGrid::NativePaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	//FVector2D TopLeft = GridBorder->GetCachedGeometry().LocalToAbsolute(FVector2D::ZeroVector);
	FVector2D TopLeft = FVector2D::ZeroVector;
	for (int i = 0; i < LINES.Num(); i++)
	{
		UWidgetBlueprintLibrary::DrawLine(Context, TopLeft + LINES[i].Start, TopLeft + LINES[i].End, FLinearColor::White, true, 1.0f);
	}
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
		bParentEnabled);
}



