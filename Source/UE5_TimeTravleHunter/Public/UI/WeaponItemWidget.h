// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponItemWidget.generated.h"

class USizeBox;
class UImage;
class UCanvasPanel;
class APickUpItem;
class UCurrentWeaponWidget;
class UEquipWeaponWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddItemWidget, UWeaponItemWidget* );
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemoveItemWidget, UWeaponItemWidget *);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UWeaponItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnAddItemWidget OnAddItemWidget;
	FOnRemoveItemWidget OnRemoveItemWidget;

	UPROPERTY(meta = (BindWidget))
	USizeBox *WeaponSizeBox;

	UPROPERTY(meta = (BindWidget))
	UImage *WeaponImage;

	UPROPERTY(meta = (BindWidget))
	UImage *HoverImage;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *WeaponCavas;

	UPROPERTY()
	APickUpItem *WeaponInfo;

	UPROPERTY()
	UCurrentWeaponWidget *CurrentWeaponItem;

	UPROPERTY()
	UEquipWeaponWidget *EquipWeaponWidget;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeWeaponItem(UEquipWeaponWidget* EquipWidget, UCurrentWeaponWidget* CurWeaponWidget, APickUpItem *WeaponItem);
	virtual void NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent &InMouseEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	
};
