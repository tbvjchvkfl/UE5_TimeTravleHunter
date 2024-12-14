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
	
	InitAnimationInstance();
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	OwnerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController());
	OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	PreviousLocation = OwnerCharacter->GetActorLocation();
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
		FootIK(DeltaSeconds);
	}
}

void UPlayerAnimInstance::InitAnimationInstance()
{
	ActualGroundSpeed = 0.0f;
	MovementSpeed = 0.0f;
	MovementStartAngle = 0.0f;
	CharacterVelocity = FVector::ZeroVector;
	CharacterMovementState = ECharacterMovementState::IDLE;
	CharacterMovementDirection = ECharacterMovementDirection::FOWARD;
	CanWalkStartSkip = false;
	CanJogStartSkip = false;
}

void UPlayerAnimInstance::SetMovementData()
{
	CharacterVelocity = OwnerCharacterMovement->Velocity;
	MovementSpeed = CharacterVelocity.Size();

	bIsInAir = OwnerCharacterMovement->IsFalling();
	bIsAccelation = OwnerCharacterMovement->GetCurrentAcceleration().Size() > 0.0f;
	bIsCrouch = OwnerController->bIsCrouch;
	bIsWalk = OwnerController->bIsWalk;
	bIsJog = OwnerController->bIsJog;
	bIsSprint = OwnerController->bIsSprint;
	bIsParkour = OwnerController->bIsParkour;

	FRotator BaseAimRotation = OwnerCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
	MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation).Yaw;

	SetMaxAccelAndPlayRate();
	SetRotationRate(0.0f, 500.0f);
	DetermineMovementDirection();
}

void UPlayerAnimInstance::SetMaxAccelAndPlayRate()
{
	float ClampValue = FMath::Clamp(GetCurveValue(FName("MoveData_Speed")), 50.0f, 1000.0f);
	PlayRate = UKismetMathLibrary::SafeDivide(MovementSpeed, ClampValue);
}

void UPlayerAnimInstance::SetRotationRate(float MinLocomotionValue, float MaxLocomotionValue)
{
	float ClampedRotValue = UKismetMathLibrary::MapRangeClamped(GetCurveValue(FName("Movement_Rotation")), 0.0f, 1.0f, MinLocomotionValue, MaxLocomotionValue);
	OwnerCharacterMovement->RotationRate = FRotator(0.0f, ClampedRotValue, 0.0f);

	OwnerCharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = false;
}

bool UPlayerAnimInstance::SetMovementDirection(float MinValue, float MaxValue, bool Mincluding, bool Maxcluding, float &Direction) const
{
	if (OwnerCharacter)
	{
		float MovementAngle = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation());

		float ClampAngle = FMath::Clamp(MovementAngle, -180.0f, 180.0f);

		if (UKismetMathLibrary::InRange_FloatFloat(ClampAngle, MinValue, MaxValue, Mincluding, Maxcluding))
		{
			Direction = ClampAngle;
			return true;
		}
		Direction = 0.0f;
		return false;
	}
	return false;
}

void UPlayerAnimInstance::DetermineMovementDirection()
{
	if (MovementSpeed <= 5.0f)
	{
		if (SetMovementDirection(-180.0f, -135.0f, true, false, MovementStartAngle))
		{
			CharacterMovementDirection = ECharacterMovementDirection::TURNLEFT_180;
			return;
		}
		else if (SetMovementDirection(-135.0f, -45.0f, true, false, MovementStartAngle))
		{
			CharacterMovementDirection = ECharacterMovementDirection::TURNLEFT_90;
			return;
		}
		else if (SetMovementDirection(45.0f, 135.0f, false, true, MovementStartAngle))
		{
			CharacterMovementDirection = ECharacterMovementDirection::TURNRIGHT_90;
			return;
		}
		else if (SetMovementDirection(135.0f, 180.0f, false, true, MovementStartAngle))
		{
			CharacterMovementDirection = ECharacterMovementDirection::TURNRIGHT_180;
			return;
		}
		else
		{
			CharacterMovementDirection = ECharacterMovementDirection::FOWARD;
			return;
		}
	}
}

void UPlayerAnimInstance::FootIK(float DeltaSecond)
{
	if (!bIsInAir)
	{
		TTuple<bool, float> Foot_L = CapsuleDistance("ik_foot_l");
		TTuple<bool, float> Foot_R = CapsuleDistance("ik_foot_r");

		if (Foot_L.Get<0>() || Foot_R.Get<0>())
		{
			const float SelectFloat = UKismetMathLibrary::SelectFloat(Foot_L.Get<1>(), Foot_R.Get<1>(), Foot_L.Get<1>() >= Foot_R.Get<1>());

			Displacement = FMath::FInterpTo(Displacement, (SelectFloat - 98.0f) * -1.0f, DeltaSecond, CurrentInterpSpeed);

			TTuple<bool, float, FVector> FootTrace_L = FootTrace("ik_foot_l");
			TTuple<bool, float, FVector> FootTrace_R = FootTrace("ik_foot_r");

			const float Distance_L = FootTrace_L.Get<1>();
			const FVector FootLVector = FootTrace_L.Get<2>();
			const FRotator MakeLRot(UKismetMathLibrary::DegAtan2(FootLVector.X, FootLVector.Z) * -1.0f, 0.0, UKismetMathLibrary::DegAtan2(FootLVector.Y, FootLVector.Z));

			L_FootRotation = FMath::RInterpTo(L_FootRotation, MakeLRot, DeltaSecond, CurrentInterpSpeed);
			L_FootIK = FMath::FInterpTo(L_FootIK, (Distance_L - 110.0f) / -45.0f, DeltaSecond, CurrentInterpSpeed);

			const float Distance_R = FootTrace_R.Get<1>();
			const FVector FootRVector = FootTrace_R.Get<2>();
			const FRotator MakeRRot(UKismetMathLibrary::DegAtan2(FootRVector.X, FootRVector.Z) * -1.0f, 0.0f, UKismetMathLibrary::DegAtan2(FootRVector.Y, FootRVector.Z));

			R_FootRotation = FMath::RInterpTo(R_FootRotation, MakeRRot, DeltaSecond, CurrentInterpSpeed);
			R_FootIK = FMath::FInterpTo(R_FootIK, (Distance_R - 110.0f) / -45.0f, DeltaSecond, CurrentInterpSpeed);
		}
	}
	else
	{
		L_FootRotation = FMath::RInterpTo(L_FootRotation, FRotator::ZeroRotator, DeltaSecond, CurrentInterpSpeed);
		L_FootIK = FMath::FInterpTo(L_FootIK, 0.0f, DeltaSecond, CurrentInterpSpeed);

		R_FootRotation = FMath::RInterpTo(R_FootRotation, FRotator::ZeroRotator, DeltaSecond, CurrentInterpSpeed);
		R_FootIK = FMath::FInterpTo(R_FootIK, 0.0f, DeltaSecond, CurrentInterpSpeed);
	}
}

TTuple<bool, float> UPlayerAnimInstance::CapsuleDistance(FName SocketName)
{
	FVector WorldLocation{ OwnerCharacter->GetMesh()->GetComponentLocation() };
	FVector BreakVector{ WorldLocation + FVector(0.0f, 0.0f, 98.0f) };

	FVector SocketLocation{ OwnerCharacter->GetMesh()->GetSocketLocation(SocketName) };
	FVector StartPos{ SocketLocation.X, SocketLocation.Y, BreakVector.Z };
	FVector EndPos{ StartPos - FVector(0.0f, 0.0f, 151.0f) };
	FHitResult CapsuleHit;
	FCollisionQueryParams CapColParams;
	CapColParams.AddIgnoredActor(OwnerCharacter);


	if (GetWorld()->LineTraceSingleByChannel(CapsuleHit, StartPos, EndPos, ECollisionChannel::ECC_Visibility, CapColParams))
	{
		return MakeTuple(true, CapsuleHit.Distance);
	}

	return MakeTuple(false, CapsuleHit.Distance);
}

TTuple<bool, float, FVector> UPlayerAnimInstance::FootTrace(FName SocketName)
{
	const FVector BoneLoc = OwnerCharacter->GetMesh()->GetSocketLocation(SocketName);
	const FVector RootLoc = OwnerCharacter->GetMesh()->GetSocketLocation("root");

	FHitResult IKHit;
	FVector StartPos{ BoneLoc.X, BoneLoc.Y, RootLoc.Z + 105.0f };
	FVector EndPos{ StartPos - FVector(0.0f, 0.0f, 105.0f) };
	FCollisionQueryParams IKColParam;
	IKColParam.AddIgnoredActor(OwnerCharacter);

	if (GetWorld()->LineTraceSingleByChannel(IKHit, StartPos, EndPos, ECollisionChannel::ECC_Visibility, IKColParam))
	{
		return MakeTuple(true, IKHit.Distance, IKHit.Normal);
	}

	return MakeTuple(false, 999.0f, FVector::ZeroVector);
}

void UPlayerAnimInstance::PlayCrouchVaulting()
{
	if (CrouchVaulting_Anim)
	{
		Montage_Play(CrouchVaulting_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayHurdling()
{
	if (Hurdling_Anim)
	{
		Montage_Play(Hurdling_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayClimbing()
{
	if (Climbing_Anim)
	{
		Montage_Play(Climbing_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayMantling()
{
	if (Mantling_Anim)
	{
		Montage_Play(Mantling_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayAssasination()
{
	if (Assasination_Anim)
	{
		Montage_Play(Assasination_Anim, 1.0f);
	}
}