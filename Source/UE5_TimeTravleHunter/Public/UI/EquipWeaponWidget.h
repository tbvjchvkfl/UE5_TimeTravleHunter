// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipWeaponWidget.generated.h"

class APlayerCharacter;
class UInventoryComponent;
class APickUpItem;
class UImage;
class UWrapBox;
class UEquipmentSlot;
class UWeaponItemWidget;
class UCurrentWeaponWidget;

DECLARE_MULTICAST_DELEGATE(FOnAddEquip);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UEquipWeaponWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnAddEquip OnAddEquip;

	// Widget
	UPROPERTY(meta = (BindWidget))
	UWrapBox *SlotWrapPanel;

	UPROPERTY(meta = (BindWidget))
	UWrapBox *WeaponWrapPanel;

	UPROPERTY(meta = (BindWidget))
	UCurrentWeaponWidget *CurrentWeaponWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UEquipmentSlot> EquipmentSlotWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UWeaponItemWidget> WeaponItemWidget;

	// Property
	UPROPERTY()
	TArray<APickUpItem *> EquipWeaponInventory;

	UPROPERTY()
	TArray<UWeaponItemWidget *> WeaponItemInventory;

	UPROPERTY()
	int32 EquipInventorySize;

	UPROPERTY()
	UInventoryComponent *InventoryComponent;

	UPROPERTY()
	APlayerCharacter *Player;

	UPROPERTY()
	UEquipmentSlot *EquipmentSlot;

	UPROPERTY()
	UWeaponItemWidget *WeaponItem;

	

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeEquipmenWidget();
	void InitEssentialData();
	void RefreshInventory();
	void FillEmptySlot();
	
	FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent &MouseEvent);
	void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent &MouseEvent);
	void OnMouseLeave(const FPointerEvent &MouseEvent);
};
