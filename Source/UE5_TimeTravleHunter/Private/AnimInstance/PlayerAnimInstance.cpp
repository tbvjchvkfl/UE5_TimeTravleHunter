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
	OwnerController = Cast<APlayerCharacterController>(OwnerCharacter->Controller);
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!OwnerController)
	{
		OwnerController = Cast<APlayerCharacterController>(OwnerCharacter->Controller);
	}
	if (OwnerController)
	{
		FVector Velocity = OwnerCharacter->GetVelocity();
		Velocity.Z = 0;

		MovementSpeed = Velocity.Size();
		bIsAccelation = OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;
		bIsInAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
		bIsCrouch = OwnerController->bIsCrouch;
		bIsWalk = OwnerController->bIsWalk;
		bIsSprint = OwnerController->bIsSprint;
		bIsAimming = OwnerController->bIsAimming;
		bIsPistol = OwnerController->bIsPistol;
		bIsRifle = OwnerController->bIsRifle;

		FRotator BaseAimRotation = OwnerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
		MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation).Yaw;
	}
	/*if (OwnerCharacter == nullptr)
	{
		OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
	if (OwnerCharacter)
	{
		FVector Velocity = OwnerCharacter->GetVelocity();
		Velocity.Z = 0;
		
		MovementSpeed = Velocity.Size();
		bIsAccelation = OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;
		bIsInAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
		bIsCrouch = OwnerCharacter->bIsCrouch;
		bIsWalk = OwnerCharacter->bIsWalk;
		bIsSprint = OwnerCharacter->bIsSprint;
		bIsAimming = OwnerCharacter->bIsAimming;
		bIsPistol = OwnerCharacter->bIsPistol;
		bIsRifle = OwnerCharacter->bIsRifle;

		FRotator BaseAimRotation = OwnerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
		MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation).Yaw;

	}*/
}
