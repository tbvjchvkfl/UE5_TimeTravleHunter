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


UCLASS()
class UE5_TIMETRAVLEHUNTER_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
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

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitGrid();
	void RefreshGrid();
	void SetUpEmptyGrid();
};
