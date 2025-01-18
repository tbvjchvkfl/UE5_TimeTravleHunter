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
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRemoveWeaponWidget, UEquipmentSlot *, int32);
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
	FORCEINLINE TArray<UEquipmentSlot *> GetWidgetList()const { return WidgetList; };
	FORCEINLINE UEquipmentSlot *GetEquipSlotMain()const { return MainWeaponSlot; };
	FORCEINLINE UEquipmentSlot *GetEquipSlotSub()const { return SubWeaponSlot; };
	FORCEINLINE UEquipmentSlot *GetEquipSlotRanged()const { return RangedWeaponSlot; };
	FORCEINLINE bool GetWeaponButtonStateMain()const { return bIsMainButtonState; };
	FORCEINLINE bool GetWeaponButtonStateSub()const { return bIsSubButtonState; };
	FORCEINLINE bool GetWeaponButtonStateRanged()const { return bIsRangedButtonState; };

	void InitializeEquipmentContents(UEquipment *EquipmentWidget);
	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime);
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	APlayerCharacter *Player;

	UPROPERTY()
	UInventoryComponent *InventoryComponent;

	UPROPERTY()
	UEquipmentSlot *EquipmentSlot;

	UPROPERTY()
	TArray<APickUpItem *> ItemList;

	UPROPERTY()
	TArray<UEquipmentSlot *> WidgetList;

	UPROPERTY()
	int32 ListSize;

	UPROPERTY()
	UEquipment *EquipWidget;

	// Equipment Widget Data
	UPROPERTY()
	UEquipmentSlot *MainWeaponSlot;

	UPROPERTY()
	UEquipmentSlot *SubWeaponSlot;

	UPROPERTY()
	UEquipmentSlot *RangedWeaponSlot;

	UPROPERTY()
	bool bIsMainButtonState;

	UPROPERTY()
	bool bIsSubButtonState;

	UPROPERTY()
	bool bIsRangedButtonState;
	
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