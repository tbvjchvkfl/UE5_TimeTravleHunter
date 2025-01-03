// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentWeaponWidget.generated.h"

class UImage;
class USizeBox;
class UEquipWeaponWidget;
class APickUpItem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipWeapon, APickUpItem *);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UCurrentWeaponWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnEquipWeapon OnEquipWeapon;

	UPROPERTY(meta = (BindWidget))
	UImage *CoverImage;

	UPROPERTY(meta = (BindWidget))
	UImage *EquipWeaponImage;

	UPROPERTY(meta = (BindWidget))
	UImage *HoverImage;

	UPROPERTY(meta = (BindWidget))
	USizeBox *ImageSizeBox;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeCurrentWeaponImage();
	void EquipWeapon(APickUpItem* WeaponItem);
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent &InMouseEvent);
};
