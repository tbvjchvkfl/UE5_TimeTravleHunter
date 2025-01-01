// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponItemWidget.generated.h"

class USizeBox;
class UImage;
class UCanvasPanel;
class AWeaponBase;


UCLASS()
class UE5_TIMETRAVLEHUNTER_API UWeaponItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(meta = (BindWidget))
	USizeBox *WeaponSizeBox;

	UPROPERTY(meta = (BindWidget))
	UImage *WeaponImage;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *WeaponCavas;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeWeaponItem(AWeaponBase *WeaponItem);
};
