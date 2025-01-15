// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Equipment.generated.h"

class UButton;
class UScrollBox;
class UTextBlock;
class APlayerCharacter;
class UInventoryComponent;
class UEquipmentSlot;
class UEquipmentContents;
class UCanvasPanel;
class UBorder;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UEquipment : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *MainWeaponCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *SubWeaponCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *RangedWeaponCanvas;

	UPROPERTY(meta = (BindWidget))
	UBorder *MainWeaponBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder *SubWeaponBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder *RangedWeaponBorder;

	UPROPERTY(meta = (BindWidget))
	UButton *MainWeaponButton;

	UPROPERTY(meta = (BindWidget))
	UButton *SubWeaponButton;

	UPROPERTY(meta = (BindWidget))
	UButton *RangedWeaponButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *WeaponName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *WeaponDescription;

	UPROPERTY(EditAnywhere, Category = "Equipment | Widget")
	TSubclassOf<UEquipmentSlot> EquipmentSlotWidget;

	UPROPERTY(EditAnywhere, Category = "Equipment | Widget")
	TSubclassOf<UEquipmentContents> EquipmentContentsWidget;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeEquipment();
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	APlayerCharacter *Player;
	UInventoryComponent *InventoryComponent;
	UEquipmentSlot *EquipmentSlot;
	UEquipmentContents *EquipmentContents;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	UFUNCTION()
	void OnMainWeaponClicked();
	UFUNCTION()
	void OnSubWeaponClicked();
	UFUNCTION()
	void OnRangedWeaponClicked();
};
