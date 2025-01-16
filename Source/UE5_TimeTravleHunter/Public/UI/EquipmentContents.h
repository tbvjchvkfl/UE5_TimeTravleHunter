// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentContents.generated.h"

class UScrollBox;
class UEquipment;
class UEquipmentSlot;
class APlayerCharacter;
class UInventoryComponent;
class APickUpItem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipWeaponWidget, UEquipmentSlot *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemoveWeaponWidget, UEquipmentSlot *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddWeaponWidget, UEquipmentSlot *);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UEquipmentContents : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnEquipWeaponWidget OnEquipWeaponWidget;
	FOnRemoveWeaponWidget OnRemoveWeaponWidget;
	FOnAddWeaponWidget OnAddWeaponWidget;

	UPROPERTY(meta = (BindWidget))
	UScrollBox *ContentsBox;

	UPROPERTY(EditAnywhere, Category = "EquipmentContents | Widget")
	TSubclassOf<UEquipmentSlot> EquipmentSlotWidget;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	FORCEINLINE UEquipment *GetEquipment()const { return EquipWidget; };

	void InitializeEquipmentContents(UEquipment *EquipmentWidget);
	
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	APlayerCharacter *Player;
	UInventoryComponent *InventoryComponent;
	UEquipmentSlot *EquipmentSlot;
	TArray<APickUpItem *> ItemList;
	TArray<UEquipmentSlot *> WidgetList;
	int32 ListSize;
	UEquipment *EquipWidget;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitEssentialData();
	void RefreshEquipmentSlot();
	void FillEmptySlot();
	void EquipWeaponWidget(UEquipmentSlot *SlotWidget);
	void RemoveWeaponWidget(UEquipmentSlot *SlotWidget);
	void AddWeaponWidget(UEquipmentSlot *SlotWidget);
};