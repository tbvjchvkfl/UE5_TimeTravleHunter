// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

class UInventoryComponent;
class APlayerCharacter;
class UInventoryGrid;
class UEquipWeaponWidget;
class UCurrentWeaponWidget;
class APickUpItem;
class UImage;

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

	UPROPERTY(meta = (BindWidget))
	UEquipWeaponWidget *EquipmentWidget;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void DropItem(FVector2D Pos);
	void RemoveItem(FVector2D Pos);
	void AddItem(FVector2D Pos, APickUpItem* Item);
	void RemoveWeaponItem(int32 Index);
	void AddWeaponItem(APickUpItem *Item);
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
