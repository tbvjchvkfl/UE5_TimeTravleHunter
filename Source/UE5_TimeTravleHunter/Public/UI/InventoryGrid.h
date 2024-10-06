// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

class UBorder;
class UCanvasPanel;
class UInventoryComponent;
class UInventoryItem;


USTRUCT()
struct FInventoryLine
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FVector2D Start;

	UPROPERTY(EditAnywhere)
	FVector2D End;
};

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(meta = (BindWidget))
	UBorder *GridBorder;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *GridCanvas;

	UPROPERTY(EditAnywhere, Category = "ItemWidget")
	TSubclassOf<UInventoryItem> InventoryItem;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void GridInitialize(class UInventoryComponent *InventoryComp, float Tilesize);

	virtual int32 NativePaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled)const override;

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	TArray<FInventoryLine> LINES;

	UPROPERTY()
	UInventoryComponent *ReferenceInventory;

	UPROPERTY()
	UInventoryItem *ItemWidget;

	UPROPERTY()
	float GridTileSize;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void CreateLine(class UInventoryComponent *InventoryComp, float Tilesize);
	void RefreshGridInventory();
	void RemoveItemInWidget(class APickUpItem *PickUpItem);
};
