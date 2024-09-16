// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/InventoryGird.h"
#include "Component/InventoryComponent.h"

// Engine
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryGird::GridInitialize(class UInventoryComponent *InventoryComp, float Tilesize)
{
	GridBorder->SetDesiredSizeScale(FVector2D(InventoryComp->GetColumns() * Tilesize, InventoryComp->GetRows() * Tilesize));
	if (auto CanvasSlot = Cast<UCanvasPanelSlot>(GridBorder->Slot))
	{
		CanvasSlot->SetSize(FVector2D(InventoryComp->GetColumns() * Tilesize, InventoryComp->GetRows() * Tilesize));
	}
	CreateLine(InventoryComp, Tilesize);
}

void UInventoryGird::CreateLine(class UInventoryComponent *InventoryComp, float Tilesize)
{
	for (int i = 0; i < InventoryComp->GetColumns(); i++)
	{
		LINES.Add(FInventoryLine(FVector2D(i * Tilesize, 0), FVector2D(i * Tilesize, InventoryComp->GetRows() * Tilesize)));
	}
	for (int i = 0; i < InventoryComp->GetRows(); i++)
	{
		LINES.Add(FInventoryLine(FVector2D(0, i * Tilesize), FVector2D(InventoryComp->GetColumns() * Tilesize, i * Tilesize)));
	}
}

int32 UInventoryGird::NativePaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const
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


