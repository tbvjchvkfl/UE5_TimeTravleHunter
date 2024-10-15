// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

class UInventoryComponent;
class APlayerCharacter;
class UInventoryGrid;
class APickUpItem;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UInventory : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(meta = (BindWidget))
	UInventoryGrid *GridWidget;


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

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeConstruct() override;
};
