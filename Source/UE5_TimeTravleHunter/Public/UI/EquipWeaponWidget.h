// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipWeaponWidget.generated.h"

class APlayerCharacter;
class UInventoryComponent;
class AWeaponBase;
class UImage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddEquip, AWeaponBase*);

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
	UImage *CurrentWeaponImage;


	// Property
	UPROPERTY()
	TArray<AWeaponBase *> EquipWeaponArray;

	UPROPERTY()
	int32 EquipInventorySize;

	UPROPERTY()
	UInventoryComponent *InventoryComponent;

	UPROPERTY()
	APlayerCharacter *Player;



	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeEquipmenWidget();
	void RefreshInventory();
	FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent &MouseEvent);
	void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent &MouseEvent);
	void OnMouseLeave(const FPointerEvent &MouseEvent);
};
