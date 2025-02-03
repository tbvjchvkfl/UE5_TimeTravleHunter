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
class UEquipment;
class UItemViewUI;
class APickUpItem;
class UImage;
class UButton;
class APickUpItemDummy;

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

	UPROPERTY(meta = (BindWidget))
	UEquipment *Equipment;

	UPROPERTY(meta = (BindWidget))
	UItemViewUI *ItemViewUI;

	UPROPERTY(meta = (BindWidget))
	UButton *BTN_Equip;

	UPROPERTY(meta = (BindWidget))
	UButton *BTN_Map;

	UPROPERTY(meta = (BindWidget))
	UButton *BTN_Inventory;

	UPROPERTY(meta = (BindWidget))
	UButton *BTN_Setting;

	UPROPERTY(EditAnywhere, Category = "Widget | Property")
	TSubclassOf<APickUpItemDummy> PickUpItemDummyClass;
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

	UPROPERTY()
	APickUpItemDummy *PickUpItemDummy;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeConstruct() override;
	void CheckItemDummyActor();
	void DesiredItemMesh(APickUpItem* Item);

	UFUNCTION()
	void ShowEquipment();
	UFUNCTION()
	void ShowInventory();
};
