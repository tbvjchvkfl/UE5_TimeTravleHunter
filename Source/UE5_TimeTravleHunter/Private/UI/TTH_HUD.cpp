// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTH_HUD.h"
#include "UI/GameHUD.h"

ATTH_HUD::ATTH_HUD()
{
	PrimaryActorTick.bCanEverTick = true;
	GameHUDWidget = nullptr;
}

void ATTH_HUD::BeginPlay()
{
	Super::BeginPlay();
	if (GameHUD)
	{
		GameHUDWidget = CreateWidget<UGameHUD>(GetWorld(), GameHUD);
		GameHUDWidget->AddToViewport();
		GameHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
