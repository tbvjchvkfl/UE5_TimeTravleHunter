// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentWeaponWidget.generated.h"

class UImage;
class USizeBox;
class UEquipWeaponWidget;
class UWeaponItemWidget;
class APickUpItem;
class APlayerCharacter;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipWeapon, UWeaponItemWidget *);

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

	UPROPERTY()
	APlayerCharacter *Player;

	UPROPERTY()
	UWeaponItemWidget *CurrentWidget;

	UPROPERTY()
	UWeaponItemWidget *PrevWidget;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeCurrentWeaponImage();
	void AddWeapon();
	void EquipWeapon(UWeaponItemWidget* Widget);
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent &InMouseEvent);
};
