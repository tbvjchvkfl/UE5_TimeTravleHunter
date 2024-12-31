// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlot.generated.h"

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
	UImage *EquipWeaponImage;

	//======================================================
	//=					- Functionary -					   =
	//======================================================

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	TArray<AWeaponBase *> EquipWeaponInventory;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeConstruct();
};
