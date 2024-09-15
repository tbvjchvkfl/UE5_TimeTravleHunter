// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TTH_HUD.generated.h"

class UGameHUD;

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

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	ATTH_HUD();
protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	UGameHUD *GameHUDWidget;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay()override;
};
