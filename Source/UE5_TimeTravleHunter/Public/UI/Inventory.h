// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

class UInventoryComponent;
class APlayerCharacter;
class UInventoryGird;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UInventory : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(meta = (BindWidget))
	UInventoryGird *InventoryGrid;


	//======================================================
	//=					- Functionary -					   =
	//======================================================

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	UInventoryComponent *InventoryComponent;

	UPROPERTY()
	APlayerCharacter *Player;

	UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
	float TileSize;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)override;

	void UpdateInventory();
};
