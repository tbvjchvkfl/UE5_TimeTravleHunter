// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Controller/PlayerCharacterController.h"
#include "Character/Player/PlayerCharacter.h"
#include "Object/PickUpItem.h"
#include "UI/TTH_HUD.h"

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
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Stop")), true);
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem *const SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(DefaultContext, 0);
	}

	if (UEnhancedInputComponent *const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(Moving, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);

		EnhancedInputComponent->BindAction(Sprinting, ETriggerEvent::Triggered, this, &APlayerCharacterController::Sprint);

		EnhancedInputComponent->BindAction(Crouching, ETriggerEvent::Triggered, this, &APlayerCharacterController::Crouch);

		EnhancedInputComponent->BindAction(Looking, ETriggerEvent::Triggered, this, &APlayerCharacterController::Look);

		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Started, this, &APlayerCharacterController::JumpStart);
		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Completed, this, &APlayerCharacterController::JumpStop);

		EnhancedInputComponent->BindAction(MoveSwitching, ETriggerEvent::Triggered, this, &APlayerCharacterController::WalktoJog);

		EnhancedInputComponent->BindAction(Interacting, ETriggerEvent::Triggered, this, &APlayerCharacterController::Interaction);

		EnhancedInputComponent->BindAction(Inventory, ETriggerEvent::Triggered, this, &APlayerCharacterController::ShowInventory);

		EnhancedInputComponent->BindAction(Aimming, ETriggerEvent::Started, this, &APlayerCharacterController::StartAimming);
		EnhancedInputComponent->BindAction(Aimming, ETriggerEvent::Completed, this, &APlayerCharacterController::StopAimming);

		EnhancedInputComponent->BindAction(SwitchingMainWeapon, ETriggerEvent::Triggered, this, &APlayerCharacterController::SwitchingWeaponMain);
		EnhancedInputComponent->BindAction(SwitchingSubWeapon, ETriggerEvent::Triggered, this, &APlayerCharacterController::SwitchingWeaponSub);
	}
}

void APlayerCharacterController::Move(const FInputActionValue &Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	PlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	PlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacterController::Look(const FInputActionValue &Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	PlayerCharacter->AddControllerYawInput(LookAxisVector.X * LookingRotationValue * GetWorld()->GetDeltaSeconds());
	PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y * LookingRotationValue * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacterController::Sprint(const FInputActionValue &Value)
{
	if (PlayerCharacter->bIsSprint == false)
	{
		PlayerCharacter->bIsSprint = true;
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	}
	else
	{
		PlayerCharacter->bIsSprint = false;
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void APlayerCharacterController::Crouch(const FInputActionValue &Value)
{
	if (PlayerCharacter->bIsCrouch == false)
	{
		PlayerCharacter->bIsCrouch = true;
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	}
	else
	{
		PlayerCharacter->bIsCrouch = false;
		if (PlayerCharacter->bIsWalk)
		{
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		}
		else
		{
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}
	}
}

void APlayerCharacterController::WalktoJog(const FInputActionValue &Value)
{
	if (PlayerCharacter->bIsWalk == false)
	{
		PlayerCharacter->bIsWalk = true;
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
	else
	{
		PlayerCharacter->bIsWalk = false;
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void APlayerCharacterController::JumpStart()
{
	PlayerCharacter->Jump();
}

void APlayerCharacterController::JumpStop()
{
	PlayerCharacter->StopJumping();
}

void APlayerCharacterController::StartAimming()
{
	if (PlayerCharacter->bIsAimming == false)
	{
		PlayerCharacter->bIsAimming = true;
		PlayerCharacter->bUseControllerRotationYaw = true;
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("Aimming")), true);
	}
}

void APlayerCharacterController::StopAimming()
{
	if (PlayerCharacter->bIsAimming)
	{
		PlayerCharacter->bIsAimming = false;
		PlayerCharacter->bUseControllerRotationYaw = false;
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		if (PlayerCharacter->bIsWalk == false)
		{
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}
	}
}

void APlayerCharacterController::SwitchingWeaponMain()
{
}

void APlayerCharacterController::SwitchingWeaponSub()
{
	if (PlayerCharacter->bIsPistol)
	{
		PlayerCharacter->bIsPistol = false;
	}
	if (PlayerCharacter->bIsRifle)
	{

	}
	if (PlayerCharacter->bIsShotgun)
	{

	}
}

void APlayerCharacterController::SprintCameraMoving()
{
	if (PlayerCharacter->bIsSprint)
	{

	}
}

void APlayerCharacterController::Interaction()
{
	FVector StartPos{ PlayerCharacter->GetPawnViewLocation() + FVector(0.0f, 0.0f, 10.0f) };
	FVector EndPos{ StartPos + (PlayerCharacter->GetViewRotation().Vector() * 1000.0f) };
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult HitTrace;
	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, 3.0f);
	if (GetWorld()->LineTraceSingleByChannel(HitTrace, StartPos, EndPos, ECollisionChannel::ECC_GameTraceChannel1, QueryParams))
	{
		PlayerCharacter->InteractionInterface = HitTrace.GetActor();
		if (HitTrace.bBlockingHit)
		{
			if (auto PickUpItem = Cast<APickUpItem>(HitTrace.GetActor()))
			{
				PlayerCharacter->InteractionInterface->PickUpInteraction(PlayerCharacter);
			}
		}
	}
}

void APlayerCharacterController::ShowInventory()
{
	OwningHUD->ToggleInventory();
}

void APlayerCharacterController::TestFunction()
{
}
