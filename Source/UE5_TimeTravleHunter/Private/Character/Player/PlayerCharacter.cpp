// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Character/Player/PlayerCharacter.h"
#include "Controller/PlayerCharacterController.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "Interface/InteractionInterface.h"
#include "Interface/CharacterActionInterface.h"
#include "Component/InventoryComponent.h"
#include "Object/PickUpItem.h"
#include "Object/VaultingWall.h"
#include "Object/HurdleWall.h"
#include "UI/TTH_HUD.h"

// Engine
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"
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
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MaxAcceleration = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->GroundFriction = 8.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 800.0f;
	
	ItemInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	CharacterMotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	OwningHUD = Cast<ATTH_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	OwningController = Cast<APlayerCharacterController>(GetController());
	OwningAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
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

		EnhancedInputComponent->BindAction(Crouching, ETriggerEvent::Triggered, this, &APlayerCharacter::DoCrouch);

		EnhancedInputComponent->BindAction(Looking, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Started, this, &APlayerCharacter::DoJump);
		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveSwitching, ETriggerEvent::Triggered, this, &APlayerCharacter::WalktoJog);

		EnhancedInputComponent->BindAction(Interacting, ETriggerEvent::Triggered, this, &APlayerCharacter::Interaction);

		EnhancedInputComponent->BindAction(Inventory, ETriggerEvent::Triggered, this, &APlayerCharacter::ShowInventory);

		EnhancedInputComponent->BindAction(Aimming, ETriggerEvent::Started, this, &APlayerCharacter::StartAimming);
		EnhancedInputComponent->BindAction(Aimming, ETriggerEvent::Completed, this, &APlayerCharacter::StopAimming);

		EnhancedInputComponent->BindAction(SwitchingMainWeapon, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchingWeaponMain);
		EnhancedInputComponent->BindAction(SwitchingSubWeapon, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchingWeaponSub);

		EnhancedInputComponent->BindAction(Test, ETriggerEvent::Triggered, this, &APlayerCharacter::TestFunction);
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

void APlayerCharacter::Sprint()
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

void APlayerCharacter::DoJump()
{
	if (OwningController->bIsParkour)
	{
		Parkour();
	}
	else
	{
		Jump();
	}
}

void APlayerCharacter::DoCrouch()
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

void APlayerCharacter::WalktoJog()
{
	if (!OwningController->bIsWalk)
	{
		OwningController->bIsWalk = true;
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	}
	else
	{
		OwningController->bIsWalk = false;
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void APlayerCharacter::Parkour()
{
	FHitResult DetectingWallHit;
	FVector DetectingStart = GetActorLocation();
	FVector DetectingEnd = DetectingStart + (GetActorRotation().Vector() * 180.0f);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), DetectingStart, DetectingEnd, FColor::Red, false, 3.0f);

	if (GetWorld()->LineTraceSingleByChannel(DetectingWallHit, DetectingStart, DetectingEnd, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams))
	{
		if (AVaultingWall* VaultingWall = Cast<AVaultingWall>(DetectingWallHit.GetActor()))
		{
			for (int32 i = 0; i <= 5; i++)
			{
				FHitResult DetectingHeightHit;
				FVector DetectingHeightStart = DetectingWallHit.Location + FVector(0.0f, 0.0f, 500.0f) + GetActorRotation().Vector() * 10.0f + GetActorRotation().Vector() * i * 20.0f;
				FVector DetectingHeightEnd = DetectingHeightStart - FVector(0.0f, 0.0f, 500.0f);

				if (GetWorld()->LineTraceSingleByChannel(DetectingHeightHit, DetectingHeightStart, DetectingHeightEnd, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams))
				{
					if (i == 0)
					{
						VaultStartPos = DetectingHeightHit.Location;
					}
					VaultMiddlePos = DetectingHeightHit.Location;
					DrawDebugLine(GetWorld(), DetectingHeightStart, DetectingHeightEnd, FColor::Green, false, 3.0f);
				}
				else
				{
					FHitResult LandingHit;
					FVector LandingStart = DetectingHeightHit.TraceStart + GetActorRotation().Vector() * 50.0f;
					FVector LnadingEnd = LandingStart - FVector(0.0f, 0.0f, 1000.0f);
					if (GetWorld()->LineTraceSingleByChannel(LandingHit, LandingStart, LnadingEnd, ECollisionChannel::ECC_Visibility, CollisionParams))
					{
						VaultLandingPos = LandingHit.Location;
						DrawDebugLine(GetWorld(), LandingStart, LnadingEnd, FColor::Blue, false, 3.0f);
						break;
					}
				}
			}
			Vaulting();
		}
		if (AHurdleWall *HurdleWall = Cast<AHurdleWall>(DetectingWallHit.GetActor()))
		{
			FHitResult HurdleHit;
			FVector HurdlingStart = DetectingWallHit.Location * GetActorRotation().Vector() * 20.0f + FVector(0.0f, 0.0f, 500.0f);
			FVector HurdlingEnd = HurdlingStart - FVector(0.0f, 0.0f, 500.0f);
			if (GetWorld()->LineTraceSingleByChannel(HurdleHit, HurdlingStart, HurdlingEnd, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams))
			{
				FHitResult HurdleLandHit;
				FVector HurdleLandStart = HurdleHit.Location * GetActorRotation().Vector() * 50.0f + FVector(0.0f, 0.0f, 500.0f);
				FVector HurdleLandEnd = HurdleLandStart - FVector(0.0f, 0.0f, 1000.0f);
				if (GetWorld()->LineTraceSingleByChannel(HurdleLandHit, HurdleLandStart, HurdleLandEnd, ECollisionChannel::ECC_Visibility, CollisionParams))
				{
					Hurdling();
				}
			}
		}
	}
}

void APlayerCharacter::Vaulting()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	SetActorEnableCollision(false);

	FMotionWarpingTarget Target;
	Target.Name = FName("Vault_Start");
	Target.Location = VaultStartPos;
	Target.Rotation = GetActorRotation();

	CharacterMotionWarping->AddOrUpdateWarpTarget(Target);

	FMotionWarpingTarget MiddleTarget;
	MiddleTarget.Name = FName("Vault_Middle");
	MiddleTarget.Location = VaultMiddlePos;
	MiddleTarget.Rotation = GetActorRotation();

	CharacterMotionWarping->AddOrUpdateWarpTarget(MiddleTarget);

	FMotionWarpingTarget LandingTarget;
	LandingTarget.Name = FName("Vault_End");
	LandingTarget.Location = VaultLandingPos;
	LandingTarget.Rotation = GetActorRotation();

	CharacterMotionWarping->AddOrUpdateWarpTarget(LandingTarget);

	if (OwningController->bIsCrouch)
	{
		OwningAnimInstance->PlayCrouchVaulting();

		GetWorld()->GetTimerManager().SetTimer(ParkourTimerHandle, this, &APlayerCharacter::TraversalEnd, OwningAnimInstance->CrouchVaulting_Anim->GetPlayLength(), false);
	}
	else
	{
		OwningAnimInstance->PlayNormalVaulting();
		GetWorld()->GetTimerManager().SetTimer(ParkourTimerHandle, this, &APlayerCharacter::TraversalEnd, OwningAnimInstance->NormalVaulting_Anim->GetPlayLength(), false);
	}
}

void APlayerCharacter::Hurdling()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	SetActorEnableCollision(false);

	OwningAnimInstance->PlayHurdling();

	GetWorld()->GetTimerManager().SetTimer(ParkourTimerHandle, this, &APlayerCharacter::TraversalEnd, OwningAnimInstance->Hurdling_Anim->GetPlayLength(), false);
}

void APlayerCharacter::Mantling()
{
	for (int32 i = 0; i < 9; i++)
	{
		FHitResult MantlingHit;
		FVector MantlingStartPos{ GetActorLocation() + FVector(0.0f, 0.0f, 150.0f) * i * 20.0f };
		FVector MantlingEndPos{ MantlingStartPos * GetActorForwardVector() * 200.0f };
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(MantlingHit, MantlingStartPos, MantlingEndPos, ECollisionChannel::ECC_GameTraceChannel2, QueryParams))
		{
			FHitResult LandingHit;
			FVector LandingStartPos = MantlingHit.Location + FVector(0.0f, 0.0f, 500.0f) + GetActorForwardVector() * 20.0f;
			FVector LandingEndPos = LandingStartPos - FVector(0.0f, 0.0f, 500.0f);
			if (GetWorld()->LineTraceSingleByChannel(LandingHit, LandingStartPos, LandingEndPos, ECollisionChannel::ECC_GameTraceChannel2, QueryParams))
			{
				GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString("Mantling"));
			}
			break;
		}
	}
}

void APlayerCharacter::TraversalEnd()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SetActorEnableCollision(true);
}

void APlayerCharacter::Assasination()
{
	TArray<AActor*> OverlappingActor;
	GetOverlappingActors(OverlappingActor);
	for (auto OverlapElem : OverlappingActor)
	{
		ActionInterface = OverlapElem;
		FVector ActionLoc;
		FRotator ActionRot;
		ActionInterface->StealthAssain(ActionLoc, ActionRot);
		
		OwningAnimInstance->PlayAssasination();

		FMotionWarpingTarget ActionTarget;
		ActionTarget.Name = FName("AssasinationWarp");
		ActionTarget.Location = ActionLoc;
		ActionTarget.Rotation = ActionRot;
		CharacterMotionWarping->AddOrUpdateWarpTarget(ActionTarget);
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
	//Assasination();
	Mantling();
}

//void APlayerCharacter::VaultMotionWarp()
//{
//	float Min = GetMesh()->GetComponentLocation().Z - 50.0f;
//	float Max = GetMesh()->GetComponentLocation().Z + 50.0f;
//
//	bool InRange = FMath::IsWithinInclusive(VaultLandingPos.Z, Min, Max);
//
//	if (OwningController->bIsParkour && InRange)
//	{
//		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
//		SetActorEnableCollision(false);
//
//		FMotionWarpingTarget Target;
//		Target.Name = FName("Vault_Start");
//		Target.Location = VaultStartPos;
//		Target.Rotation = GetActorRotation();
//
//		CharacterMotionWarping->AddOrUpdateWarpTarget(Target);
//
//		FMotionWarpingTarget MiddleTarget;
//		MiddleTarget.Name = FName("Vault_Middle");
//		MiddleTarget.Location = VaultMiddlePos;
//		MiddleTarget.Rotation = GetActorRotation();
//
//		CharacterMotionWarping->AddOrUpdateWarpTarget(MiddleTarget);
//
//		FMotionWarpingTarget LandingTarget;
//		LandingTarget.Name = FName("Vault_End");
//		LandingTarget.Location = VaultLandingPos;
//		LandingTarget.Rotation = GetActorRotation();
//
//		CharacterMotionWarping->AddOrUpdateWarpTarget(LandingTarget);
//
//		OwningAnimInstance->PlayVaulting();
//
//		GetWorld()->GetTimerManager().SetTimer(VaultTimerHandle, this, &APlayerCharacter::VaultEnd, OwningAnimInstance->Vaulting_Anim->GetPlayLength(), false);
//
//		SetActorEnableCollision(true);
//		OwningController->bIsParkour = false;
//		VaultLandingPos = FVector(0.0f, 0.0f, 20000.0f);
//	}
//}