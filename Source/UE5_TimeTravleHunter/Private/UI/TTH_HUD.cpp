// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTH_HUD.h"
#include "UI/GameHUD.h"
#include "UI/Inventory.h"
#include "Kismet/GameplayStatics.h"

ATTH_HUD::ATTH_HUD()
{
	PrimaryActorTick.bCanEverTick = true;
	GameHUDWidget = nullptr;
	bIsInventory = true;
}

void ATTH_HUD::ToggleInventory()
{
	if (bIsInventory)
	{
		bIsInventory = false;
		
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI GameAndUIInputMode;
		//GetOwningPlayerController()->SetPause(true);
		GetOwningPlayerController()->SetInputMode(GameAndUIInputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
		
	}
	else
	{
		bIsInventory = true;

		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

		FInputModeGameOnly GameOnlyInputMode;
		//GetOwningPlayerController()->SetPause(false);
		GetOwningPlayerController()->SetInputMode(GameOnlyInputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
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
	if (Inventory)
	{
		InventoryWidget = CreateWidget<UInventory>(GetWorld(), Inventory);
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);          
	}
}
