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
	bIsSprint(false)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

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

	/*{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputContext(TEXT("/Game/TimeTravleHunter/Input/IMC_DefaultContext"));
		if (InputContext.Succeeded())
		{
			DefaultContext = InputContext.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction> MoveAction(TEXT("/Game/TimeTravleHunter/Input/Action/IA_Move"));
		if (MoveAction.Succeeded())
		{
			Moving = MoveAction.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> JumpAction(TEXT("/Game/TimeTravleHunter/Input/Action/IA_Jump"));
		if (JumpAction.Succeeded())
		{
			Moving = JumpAction.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> LookAction(TEXT("/Game/TimeTravleHunter/Input/Action/IA_Look"));
		if (LookAction.Succeeded())
		{
			Moving = LookAction.Object;
		}
	}*/
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
	if (auto *const Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(Moving, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		
		Input->BindAction(Looking, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		Input->BindAction(Jumping, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		Input->BindAction(Jumping, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
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
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Move")), true);
	}
}

void APlayerCharacter::Look(const FInputActionValue &Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Sprint(const FInputActionValue &Value)
{
	bIsSprint = true;
	if (bIsSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

