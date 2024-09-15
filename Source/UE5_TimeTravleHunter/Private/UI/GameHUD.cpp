// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/GameHUD.h"
#include "Character/Player/PlayerCharacter.h"

// Engine
#include "Kismet/GameplayStatics.h"

// Components
#include "Components/ProgressBar.h"

void UGameHUD::NativeConstruct()
{
	Super::NativeConstruct();
	OwnerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	HealthGuage->SetPercent(OwnerCharacter->GetCurrentHealth() / OwnerCharacter->GetMaxHealth());
	StaminaGuage->SetPercent(OwnerCharacter->GetCurrentStamina() / OwnerCharacter->GetMaxStamina());
}

float UGameHUD::SetHealthGuage(float NewHealthGuage)
{
	OwnerCharacter->GetCurrentHealth();
	return 0.0f;
}

float UGameHUD::SetStaminaGuage(float NewStaminaGuage)
{
	return 0.0f;
}
