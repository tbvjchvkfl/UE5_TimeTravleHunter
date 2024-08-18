// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Character/Player/PlayerCharacter.h"
#include "Controller/PlayerCharacterController.h"

// Engine
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

APlayerCharacter::APlayerCharacter() : 
	MaxHealth(100.0f), 
	CurrentHealth(MaxHealth), 
	AbilityStamina(100.0f), 
	LookingRotationValue(0.0f), 
	bIsSprint(false)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.0f;
	CameraBoom->bUsePawnControlRotation = true;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(CameraBoom);
	MainCamera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 1.0f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (auto *const PlayerController = Cast<APlayerCharacterController>(GetController()))
	{
		if (auto *const SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultContext, 0);
		}
	}
	if (auto *const EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Moving, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		
		EnhancedInputComponent->BindAction(Sprinting, ETriggerEvent::Started, this, &APlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(Sprinting, ETriggerEvent::Completed, this, &APlayerCharacter::Sprint);

		EnhancedInputComponent->BindAction(Looking, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);


	}
}

void APlayerCharacter::Move(const FInputActionValue &Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue &Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X * LookingRotationValue);
		AddControllerPitchInput(LookAxisVector.Y * LookingRotationValue);
	}
}

void APlayerCharacter::Sprint(const FInputActionValue &Value)
{
	if (bIsSprint)
	{
		bIsSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}