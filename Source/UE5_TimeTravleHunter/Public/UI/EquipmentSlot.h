// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlot.generated.h"

class UEquipment;
class UEquipmentContents;
class APickUpItem;
class AWeaponBase;
class UImage;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(meta = (BindWidget))
	UImage *WeaponImage;

	UPROPERTY(meta = (BindWidget))
	UImage *HoverImage;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	FORCEINLINE APickUpItem *GetWeaponItem() const { return WeaponItem; };

	void InitializeEquipmentSlot(UEquipmentContents *EquipContentsWidget);
	void GetItemInfo(APickUpItem* Item);
	
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	TArray<AWeaponBase *> EquipWeaponInventory;
	APickUpItem *WeaponItem;
	UEquipmentContents *EquipContents;
	UEquipment *EquipmentWidget;

	bool MainWeaponSlotState;
	bool SubWeaponSlotState;
	bool RangedWeaponSlotState;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent &InMouseEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
};
