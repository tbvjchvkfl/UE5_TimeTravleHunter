// Fill out your copyright notice in the Description page of Project Settings.



// GameFramework
#include "UI/TTH_HUD.h"
#include "UI/GameHUD.h"
#include "UI/Inventory.h"
#include "Character/Controller/PlayerCharacterController.h"


// Engine
#include "Kismet/GameplayStatics.h"

ATTH_HUD::ATTH_HUD()
{
	PrimaryActorTick.bCanEverTick = true;
	GameHUDWidget = nullptr;
	bIsPaused = false;
}

void ATTH_HUD::ToggleInventory()
{
	if (bIsPaused)
	{
		HideInventory();
	}
	else
	{
		ShowInventory();
	}
}

void ATTH_HUD::ShowInventory()
{
	bIsPaused = true;

	InventoryWidget->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI GameAndUIMode;

	GameAndUIMode.SetWidgetToFocus(InventoryWidget->TakeWidget());

	Controller->SetInputMode(GameAndUIMode);
	Controller->SetShowMouseCursor(true);

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ATTH_HUD::HideInventory()
{
	bIsPaused = false;

	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

	FInputModeGameOnly GameOnlyInputMode;

	Controller->SetInputMode(GameOnlyInputMode);
	Controller->SetShowMouseCursor(false);

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void ATTH_HUD::BeginPlay()
{
	Super::BeginPlay();

	Controller = Cast<APlayerCharacterController> (GetOwningPlayerController());

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