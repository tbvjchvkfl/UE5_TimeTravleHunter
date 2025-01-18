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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemoveWidget, UEquipmentSlot *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddWidget, UEquipmentSlot *);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnRemoveWidget OnRemoveWidget;
	FOnAddWidget OnAddWidget;

	UPROPERTY(meta = (BindWidget))
	UImage *WeaponImage;

	UPROPERTY(meta = (BindWidget))
	UImage *HoverImage;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	FORCEINLINE APickUpItem *GetWeaponItem() const { return ItemInfo; };

	void InitializeEquipmentSlot(UEquipmentContents *EquipContentsWidget, APickUpItem* WeaponItem);
	void SetItemInfo(APickUpItem* Item);

	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime);
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	APickUpItem *ItemInfo;

	UPROPERTY()
	UEquipmentContents *EquipContents;

	UPROPERTY()
	bool MainWeaponSlotState;

	UPROPERTY()
	bool SubWeaponSlotState;

	UPROPERTY()
	bool RangedWeaponSlotState;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent &InMouseEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
};
