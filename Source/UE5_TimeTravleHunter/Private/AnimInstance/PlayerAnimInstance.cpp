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
	}
}

void UPlayerAnimInstance::SetMovementData()
{
	CharacterVelocity = OwnerCharacterMovement->Velocity;
	MovementSpeed = CharacterVelocity.Size();
	bIsInAir = OwnerCharacterMovement->IsFalling();
	bIsAccelation = OwnerCharacterMovement->GetCurrentAcceleration().Size() > 0.0f;
	bIsCrouch = OwnerController->bIsCrouch;
	bIsWalk = OwnerController->bIsWalk;
	bIsSprint = OwnerController->bIsSprint;
	bIsParkour = OwnerController->bIsParkour;

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, FString::Printf(TEXT("Last Input Vector Size : %f"), OwnerCharacterMovement->GetLastInputVector().Size()), true);

	FVector InputVector = OwnerCharacterMovement->GetLastInputVector();
	InputVector.GetClampedToSize(0.0f, 1.0f);
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString::Printf(TEXT("Clamp Last Input Vector Size : %f, %f, %f"), InputVector.X, InputVector.Y, InputVector.Z));

	GEngine->AddOnScreenDebugMessage(3, 3, FColor::Green, FString::Printf(TEXT("CharacterVelocity : %f, %f, %f"), CharacterVelocity.X, CharacterVelocity.Y, CharacterVelocity.Z));

	FVector NormCV = CharacterVelocity.GetSafeNormal();
	GEngine->AddOnScreenDebugMessage(4, 3, FColor::Green, FString::Printf(TEXT("Normal CharacterVelocity : %f, %f, %f"), NormCV.X, NormCV.Y, NormCV.Z));

	GEngine->AddOnScreenDebugMessage(5, 3, FColor::Green, FString::Printf(TEXT("CurrentAcceleration : %f, %f, %f"), OwnerCharacterMovement->GetCurrentAcceleration().X, OwnerCharacterMovement->GetCurrentAcceleration().Y, OwnerCharacterMovement->GetCurrentAcceleration().Z));

	auto NormAcc = OwnerCharacterMovement->GetCurrentAcceleration().GetSafeNormal();
	GEngine->AddOnScreenDebugMessage(6, 3, FColor::Green, FString::Printf(TEXT("Normal CurrentAcceleration : %f, %f, %f"), NormAcc.X, NormAcc.Y, NormAcc.Z));
	
	double DotV = FVector::DotProduct(NormCV, NormAcc);
	GEngine->AddOnScreenDebugMessage(7, 3, FColor::Green, FString::Printf(TEXT("Dot Product Vector : %f"), DotV));

	if (DotV < -0.5f)
	{
		GEngine->AddOnScreenDebugMessage(8, 3, FColor::Green, FString::Printf(TEXT("true")));
	}

	FRotator BaseAimRotation = OwnerCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
	MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation).Yaw;
}

void UPlayerAnimInstance::DetermineLocomotionState()
{
	if (bIsWalk && bIsAccelation && !bIsInAir)
	{
		CharacterMovementState = ECharacterMovementState::WALK;
	}
	if (!bIsWalk && bIsAccelation && !bIsInAir && MovementSpeed >= 250.0f)
	{
		CharacterMovementState = ECharacterMovementState::JOG;
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