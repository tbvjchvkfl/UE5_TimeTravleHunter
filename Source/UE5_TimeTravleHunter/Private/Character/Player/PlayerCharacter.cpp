// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Character/Player/PlayerCharacter.h"
#include "Controller/PlayerCharacterController.h"
#include "Interface/InteractionInterface.h"
#include "Component/InventoryComponent.h"
#include "Object/PickUpItem.h"
#include "UI/TTH_HUD.h"

// Engine
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter() : 
	MaxHealth(100.0f), 
	CurrentHealth(MaxHealth),  
	MaxStamina(80.0f),
	CurrentStamina(MaxStamina),
	LookingRotationValue(1.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.0f;
	CameraBoom->bUsePawnControlRotation = true;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(CameraBoom);
	MainCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 1.0f;

	ItemInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	OwningHUD = Cast<ATTH_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	OwningController = Cast<APlayerCharacterController>(GetController());
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetCharacterMovement()->IsFalling())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Falling")), true);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerCharacterController *PlayerController = Cast<APlayerCharacterController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem *const SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultContext, 0);
		}
	}

	if (UEnhancedInputComponent *const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(Moving, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		EnhancedInputComponent->BindAction(Sprinting, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);

		EnhancedInputComponent->BindAction(Sprinting, ETriggerEvent::Triggered, this, &APlayerCharacter::Vaulting);

		EnhancedInputComponent->BindAction(Crouching, ETriggerEvent::Triggered, this, &APlayerCharacter::Crouch);

		EnhancedInputComponent->BindAction(Looking, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveSwitching, ETriggerEvent::Triggered, this, &APlayerCharacter::WalktoJog);

		EnhancedInputComponent->BindAction(Interacting, ETriggerEvent::Triggered, this, &APlayerCharacter::Interaction);

		EnhancedInputComponent->BindAction(Inventory, ETriggerEvent::Triggered, this, &APlayerCharacter::ShowInventory);

		EnhancedInputComponent->BindAction(Aimming, ETriggerEvent::Started, this, &APlayerCharacter::StartAimming);
		EnhancedInputComponent->BindAction(Aimming, ETriggerEvent::Completed, this, &APlayerCharacter::StopAimming);

		EnhancedInputComponent->BindAction(SwitchingMainWeapon, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchingWeaponMain);
		EnhancedInputComponent->BindAction(SwitchingSubWeapon, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchingWeaponSub);
	}
}

void APlayerCharacter::Move(const FInputActionValue &Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue &Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X * LookingRotationValue * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(LookAxisVector.Y * LookingRotationValue * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Sprint(const FInputActionValue &Value)
{
	if (!OwningController->bIsSprint)
	{
		OwningController->bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	}
	else
	{
		OwningController->bIsSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void APlayerCharacter::Crouch(const FInputActionValue &Value)
{
	if (!OwningController->bIsCrouch)
	{
		OwningController->bIsCrouch = true;
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	}
	else
	{
		OwningController->bIsCrouch = false;
		if (OwningController->bIsWalk)
		{
			GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}
	}
}

void APlayerCharacter::WalktoJog(const FInputActionValue &Value)
{
	if (!OwningController->bIsWalk)
	{
		OwningController->bIsWalk = true;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
	else
	{
		OwningController->bIsWalk = false;
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void APlayerCharacter::Vaulting()
{
	for (int32 i = 0; i <= 2; i++)
	{
		FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, i * 30);
		FVector End = Start + (GetActorRotation().Vector() * 180.0f);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		TArray<AActor *> IgnoreActorList;
		IgnoreActorList.Add(this);

		/*DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f);

		if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
		{
			for (int32 j = 0; j < 6; j++)
			{
				FVector SecondStart = HitResult.Location + FVector(0.0f, 0.0f, 100.0f) + (GetActorRotation().Vector() * j * 50);
				FVector SecondEnd = SecondStart - FVector(0.0f, 0.0f, 100.0f);
				FHitResult SecondHit;
				FCollisionQueryParams SecondParams;
				SecondParams.AddIgnoredActor(this);

				if(GetWorld()->LineTraceSingleByChannel(SecondHit, SecondStart, SecondEnd, ECC_Visibility, SecondParams))
				{

				}
				DrawDebugLine(GetWorld(), SecondStart, SecondEnd, FColor::Blue, false, 3.0f);
			}
			break;
		}*/
		
		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 5.0f, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActorList, EDrawDebugTrace::ForDuration, HitResult, true))
		{
			for (int32 j = 0; j <= 5; j++)
			{
				FVector SubStart = HitResult.Location + FVector(0.0f, 0.0f, 100.0f) + (GetActorRotation().Vector() * j * 50);
				FVector SubEnd = SubStart - FVector(0.0f, 0.0f, 100.0f);

				FHitResult SubHit;

				if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), SubStart, SubEnd, 5.0f, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActorList, EDrawDebugTrace::ForDuration, SubHit, true))
				{
					if (j == 0)
					{
						FVector VaultStartPos = SubHit.Location;
						
						DrawDebugSphere(GetWorld(), VaultStartPos, 10.0f, 12, FColor::Blue, false, 10.0f);
					}
					FVector VaultMiddlePos = SubHit.Location;
					DrawDebugSphere(GetWorld(), VaultMiddlePos, 10.0f, 12, FColor::Yellow, false, 10.0f);

				}
				else
				{
					FHitResult LineHit;
					FVector LineStart = SubHit.TraceStart + GetActorRotation().Vector() * 80.0f;
					FVector LineEnd = LineStart - FVector(0.0f, 0.0f, 1000.0f);
					FCollisionQueryParams LineCollisionParam;
					LineCollisionParam.AddIgnoredActor(this);

					if (GetWorld()->LineTraceSingleByChannel(LineHit, LineStart, LineEnd, ECC_Visibility, LineCollisionParam))
					{
						FVector LandingPos = LineHit.Location;

						DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Black, false, 3.0f);

						break;
					}
					
					
				}
			}
			break;
		}
	}
}

void APlayerCharacter::StartAimming()
{
	if (!OwningController->bIsAimming)
	{
		OwningController->bIsAimming = true;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("Aimming")), true);
	}
}

void APlayerCharacter::StopAimming()
{
	if (OwningController->bIsAimming)
	{
		OwningController->bIsAimming = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		if (!OwningController->bIsWalk)
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}
	}
}

void APlayerCharacter::SwitchingWeaponMain()
{

}

void APlayerCharacter::SwitchingWeaponSub()
{
	if (OwningController->bIsPistol)
	{
		OwningController->bIsPistol = false;
	}
	if (OwningController->bIsRifle)
	{

	}
	if (OwningController->bIsShotgun)
	{

	}
}

void APlayerCharacter::SprintCameraMoving()
{
	if (OwningController->bIsSprint)
	{

	}
}

void APlayerCharacter::Interaction()
{
	FVector StartPos{ GetPawnViewLocation() + FVector(0.0f, 0.0f, 10.0f) };
	FVector EndPos{ StartPos + (GetViewRotation().Vector() * 1000.0f) };
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult HitTrace;
	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, 3.0f);
	if (GetWorld()->LineTraceSingleByChannel(HitTrace, StartPos, EndPos, ECollisionChannel::ECC_GameTraceChannel1, QueryParams))
	{
		InteractionInterface = HitTrace.GetActor();
		if (HitTrace.bBlockingHit)
		{
			if (auto PickUpItem = Cast<APickUpItem>(HitTrace.GetActor()))
			{
				InteractionInterface->PickUpInteraction(this);
			}
		}
	}
}

void APlayerCharacter::ShowInventory()
{
	OwningHUD->ToggleInventory();
}

void APlayerCharacter::TestFunction()
{

}
