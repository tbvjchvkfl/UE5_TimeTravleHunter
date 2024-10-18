// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.generated.h"

class APickUpItem;
class USizeBox;
class UCanvasPanel;
class UImage;
class UTextBlock;
class UItemVisibility;
class UDropDown;
class UGridSlot;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoved, UInventoryItem *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnReMoved, UInventoryItem *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDropped, UInventoryItem *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAdded, UInventoryItem *);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UInventoryItem : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnMoved OnMoved;
	FOnReMoved OnRemoved;
	FOnDropped OnDropped;
	FOnAdded OnAdded;
	//FOnRemovedItem OnRemoveItem;

	UPROPERTY(meta = (BindWidget))
	USizeBox *BackGroundSizeBox;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *WidgetCanvas;

	UPROPERTY(meta = (BindWidget))
	UImage *ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *ItemQuantityText;

	UPROPERTY(EditAnywhere, Category = "ItemWidget | Widget Reference")
	TSubclassOf<UItemVisibility> ItemVisibility;

	UPROPERTY(EditAnywhere, Category = "ItemWidget | Widget Reference")
	TSubclassOf<UDropDown> DropDown;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeInventoryItem(APickUpItem* PickUpItem);
	void UpdateVisual(APickUpItem *Item, UGridSlot *GridSlot);

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "ItemWidget | Property", meta = (AllowPrivateAccess = "true"))
	float ItemGridSize;

	UPROPERTY()
	UItemVisibility *VisibilityWidget;

	UPROPERTY()
	UDropDown *DropDownWidget;

	UPROPERTY()
	TArray<UItemVisibility *> VisibilityContainer;

	UPROPERTY()
	FVector2D Location;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void MouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	bool GetMousePosition(FVector2D &MousePos) const;
	void DropItem();
	void RemoveItem();
	void HoverItem();
	void UnHoverItem();
	void RemoveDropDown();
	void StartMovingItem();
	void StopMovingItem();
	bool IsOverlapping(FVector2D CurrentLocation, APickUpItem* Item) const;
	void GetCurrentGridLocation(float &LocationX, float &LocationY) const;
};
