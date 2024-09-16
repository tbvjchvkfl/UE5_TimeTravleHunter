// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TTH_HUD.generated.h"

class UGameHUD;
class UInventory;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API ATTH_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditDefaultsOnly, Category = "UI|Widget")
	TSubclassOf<UGameHUD> GameHUD;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Widget")
	TSubclassOf<UInventory> Inventory;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	ATTH_HUD();

	void ToggleInventory();
protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	UGameHUD *GameHUDWidget;

	UPROPERTY()
	UInventory *InventoryWidget;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay()override;

private:
	bool bIsInventory;
};
