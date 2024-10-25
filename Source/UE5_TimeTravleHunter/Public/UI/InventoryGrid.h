// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

class UGridPanel;
class UInventoryComponent;
class UInventoryItem;
class UInventorySlot;
class APickUpItem;
class APlayerCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDropItem, FVector2D);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemoveItem, FVector2D);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAddItem, FVector2D, APickUpItem *);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnDropItem OnDropItem;
	FOnRemoveItem OnRemoveItem;
	FOnAddItem OnAddItem;

	UPROPERTY(meta = (BindWidget))
	UGridPanel *InventoryGrid;

	UPROPERTY(EditAnywhere, Category = "ItemWidget")
	TSubclassOf<UInventoryItem> InventoryItem;

	UPROPERTY(EditAnywhere, Category = "ItemWidget")
	TSubclassOf<UInventorySlot> InventorySlot;

	UPROPERTY(EditAnywhere, Category = "ItemWidget")
	TMap<FVector2D, APickUpItem *> WidgetInventory;

	UPROPERTY(VisibleAnywhere, Category = "ItemWidget")
	UInventoryComponent *InventoryComponent;

	UPROPERTY(VisibleAnywhere, Category = "ItemWidget")
	int32 InventorySize;

	UPROPERTY(VisibleAnywhere, Category = "ItemWidget")
	int32 InventoryWidth;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InventoryGridInitialize();

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	APlayerCharacter *Player;

	UPROPERTY()
	UInventoryItem *ItemWidget;

	UPROPERTY()
	UInventorySlot *SlotWidget;

	UPROPERTY()
	TArray<UInventorySlot *> GridSlot;

	UPROPERTY()
	TMap<FVector2D, UInventoryItem *> GridInventory;

	UPROPERTY()
	TMap<FVector2D, bool> GridState;

	UPROPERTY()
	FVector2D CurrentLocation;

	UPROPERTY()
	UInventoryItem *MovingItem;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitGrid();
	void RefreshGrid();
	void SetUpEmptyGrid();
	void ItemAdded(UInventoryItem *WidgetItem);
	void ItemRemoved(UInventoryItem *WidgetItem);
	void ItemDropped(UInventoryItem *WidgetItem);
	void ItemMoved(UInventoryItem *WidgetItem);
	void MouseEnter(FVector2D Pos);
	void LeftMouseButtonPressed(FVector2D Pos);
	void RightMouseButtonPressed(FVector2D Pos);
	bool CanPlaceItem() const;
	void ItemMovedStop();
	void AddToInventory(UInventoryItem *WidgetItem);
	void RemoveFromInventory(FVector2D WidgetLoc);
	void AddToState(FVector2D WidgetLoc, APickUpItem *Item);
	void RemoveFromState(FVector2D WidgetLoc, APickUpItem *Item);
};
