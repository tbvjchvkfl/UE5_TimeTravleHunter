// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "AnimInstance/PlayerAnimInstance.h"
#include "Controller/PlayerCharacterController.h"
#include "Character/Player/PlayerCharacter.h"

// Engine
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	OwnerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController());
	if (OwnerCharacter)
	{
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
	CharacterMovementState = ECharacterMovementState::IDLE;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!OwnerController)
	{
		OwnerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController());
	}
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
	if (OwnerCharacter && !OwnerCharacterMovement)
	{
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
	if (OwnerCharacter && OwnerController && OwnerCharacterMovement)
	{
		SetMovementData();
		DetermineLocomotionState();
	}
}

void UPlayerAnimInstance::SetMovementData()
{
	CharacterVelocity = OwnerCharacterMovement->Velocity;
	MovementSpeed = CharacterVelocity.Size();
	LastInputVector = OwnerCharacterMovement->GetLastInputVector().GetClampedToSize(0.0f, 1.0f);

	StartRotation = OwnerCharacter->GetActorRotation();
	MainRotation = UKismetMathLibrary::MakeRotFromX(LastInputVector);

	bIsInAir = OwnerCharacterMovement->IsFalling();
	bIsAccelation = OwnerCharacterMovement->GetCurrentAcceleration().Size() > 0.0f;
	bIsCrouch = OwnerController->bIsCrouch;
	bIsWalk = OwnerController->bIsWalk;
	bIsSprint = OwnerController->bIsSprint;
	bIsParkour = OwnerController->bIsParkour;

	

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, FString::Printf(TEXT("CharacterVelocity : %f, %f, %f"), CharacterVelocity.X, CharacterVelocity.Y, CharacterVelocity.Z));

	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString::Printf(TEXT("Movement Speed : %f"), MovementSpeed), true);

	GEngine->AddOnScreenDebugMessage(2, 3, FColor::Green, FString::Printf(TEXT("Last Input Vector : %f, %f, %f"), OwnerCharacterMovement->GetLastInputVector().X, OwnerCharacterMovement->GetLastInputVector().Y, OwnerCharacterMovement->GetLastInputVector().Z));

	GEngine->AddOnScreenDebugMessage(3, 3, FColor::Green, FString::Printf(TEXT("Last Input Vector Size : %f"), OwnerCharacterMovement->GetLastInputVector().Size()), true);

	GEngine->AddOnScreenDebugMessage(4, 3, FColor::Green, FString::Printf(TEXT("Clamp Last Input Vector : %f, %f, %f"), LastInputVector.X, LastInputVector.Y, LastInputVector.Z));

	FVector TestInputVector = OwnerCharacterMovement->GetLastInputVector();
	TestInputVector.GetClampedToSize(0.0f, 1.0f);
	GEngine->AddOnScreenDebugMessage(5, 3, FColor::Green, FString::Printf(TEXT("Clamp Last Input Vector Size : %f, %f, %f"), TestInputVector.X, TestInputVector.Y, TestInputVector.Z));

	FRotator BaseAimRotation = OwnerCharacter->GetBaseAimRotation();
	GEngine->AddOnScreenDebugMessage(5, 3, FColor::Green, FString::Printf(TEXT("Base Aim Rotation : %f, %f, %f"), BaseAimRotation.Roll, BaseAimRotation.Pitch, BaseAimRotation.Yaw));

	GEngine->AddOnScreenDebugMessage(8, 3, FColor::Green, FString::Printf(TEXT("Actor Rotation : %f, %f, %f"), StartRotation.Roll, StartRotation.Pitch, StartRotation.Yaw));

	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
	GEngine->AddOnScreenDebugMessage(6, 3, FColor::Green, FString::Printf(TEXT("Character Velocity X Rotation : %f, %f, %f"), MovementRotation.Roll, MovementRotation.Pitch, MovementRotation.Yaw));

	GEngine->AddOnScreenDebugMessage(9, 3, FColor::Green, FString::Printf(TEXT("Last Input Vector X Rotation : %f, %f, %f"), MainRotation.Roll, MainRotation.Pitch, MainRotation.Yaw));

	MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation).Yaw;
	GEngine->AddOnScreenDebugMessage(7, 3, FColor::Green, FString::Printf(TEXT("MovementYawDelta : %f"), MovementYawDelta));

	WalkStartAngle = UKismetMathLibrary::NormalizedDeltaRotator(StartRotation, MainRotation).Yaw;
	GEngine->AddOnScreenDebugMessage(7, 3, FColor::Green, FString::Printf(TEXT("Walk Start Angle : %f"), WalkStartAngle));
}

void UPlayerAnimInstance::DetermineLocomotionState()
{
	if (bIsWalk && !bIsInAir && MovementSpeed >= 10.0f)
	{
		CharacterMovementState = ECharacterMovementState::WALK;
	}
	else if (!bIsWalk && !bIsInAir && MovementSpeed >= 250.0f)
	{
		CharacterMovementState = ECharacterMovementState::JOG;
	}
	else if (bIsInAir)
	{
		CharacterMovementState = ECharacterMovementState::JUMP;
	}
	else if (bIsCrouch)
	{
		CharacterMovementState = ECharacterMovementState::CROUCH;
	}
	else
	{
		CharacterMovementState = ECharacterMovementState::IDLE;
	}
}

void UPlayerAnimInstance::PlayCrouchVaulting()
{
	if (CrouchVaulting_Anim)
	{
		Montage_Play(CrouchVaulting_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayNormalVaulting()
{
	if (NormalVaulting_Anim)
	{
		Montage_Play(NormalVaulting_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayHurdling()
{
	if (Hurdling_Anim)
	{
		Montage_Play(Hurdling_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayAssasination()
{
	if (Assasination_Anim)
	{
		Montage_Play(Assasination_Anim, 1.0f);
	}
}