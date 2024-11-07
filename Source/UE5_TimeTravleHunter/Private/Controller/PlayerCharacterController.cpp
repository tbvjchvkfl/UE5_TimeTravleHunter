// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Controller/PlayerCharacterController.h"
#include "Character/Player/PlayerCharacter.h"
#include "Object/PickUpItem.h"
#include "UI/TTH_HUD.h"
#include "UI/Inventory.h"

// Engine
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacterController::APlayerCharacterController() : LookingRotationValue(1.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	OwningHUD = Cast<ATTH_HUD>(GetHUD());
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	}
}