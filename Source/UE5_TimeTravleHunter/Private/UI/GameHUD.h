// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

class UProgressBar;
class APlayerCharacter;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(meta = (BindWidget))
	UProgressBar *HealthGuage;

	UPROPERTY(meta = (BindWidget))
	UProgressBar *StaminaGuage;

	UPROPERTY()
	APlayerCharacter *OwnerCharacter;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeConstruct() override;

	float SetHealthGuage(float NewHealthGuage);
	float SetStaminaGuage(float NewStaminaGuage);
};
