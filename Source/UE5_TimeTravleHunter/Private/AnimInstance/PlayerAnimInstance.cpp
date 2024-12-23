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
	
	AddNativeStateEntryBinding(TEXT("LocomotionState"), TEXT("Move_Start"), FOnGraphStateChanged::CreateUObject(this, &UPlayerAnimInstance::OnEntryMoveStartState));

	AddNativeStateEntryBinding(TEXT("LocomotionState"), TEXT("Move_Stop"), FOnGraphStateChanged::CreateUObject(this, &UPlayerAnimInstance::OnEntryMoveStopState));
	
	AddNativeStateEntryBinding(TEXT("LocomotionState"), TEXT("Pivot_Turn"), FOnGraphStateChanged::CreateUObject(this, &UPlayerAnimInstance::OnEntryPivotTurnState));
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	OwnerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController());
	OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();

	PreviousUnitVector = OwnerCharacter->GetActorForwardVector();
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
	MovementSpeed = 0.0f;
	MovementStartAngle = 0.0f;
	CharacterVelocity = FVector::ZeroVector;
	PreviousUnitVector = FVector::ZeroVector;
	CanWalkStartSkip = false;
	CanJogStartSkip = false;
	bIsTurn = false;
}

void UPlayerAnimInstance::SetMovementData()
{
	CharacterVelocity = OwnerCharacterMovement->Velocity;
	MovementSpeed = CharacterVelocity.Size();

	bIsInAir = OwnerCharacterMovement->IsFalling();
	bIsAccelation = OwnerCharacterMovement->GetCurrentAcceleration().Size() > 0.0f;
	bIsCrouch = OwnerController->bIsCrouch;
	bIsJog = OwnerController->bIsJog;
	bIsSprint = OwnerController->bIsSprint;
	bIsParkour = OwnerController->bIsParkour;
	bIsTurn = false;
	MovementElapsedTime = OwnerCharacter->MoveElapsedTime;

	
	
	GEngine->AddOnScreenDebugMessage(8, 3, FColor::Green, FString::Printf(TEXT("CharacterForwardVector : %f, %f, %f"), OwnerCharacter->GetActorForwardVector().X, OwnerCharacter->GetActorForwardVector().Y, OwnerCharacter->GetActorForwardVector().Z));
	MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity()), OwnerCharacter->GetBaseAimRotation()).Yaw;

	SetMaxSpeedAndPlayRate();
	CheckCurrentDirection();
	SetRotationRate(0.0f, 500.0f);

	GEngine->AddOnScreenDebugMessage(2, 3, FColor::Green, FString::Printf(TEXT("MovementSpeed : %f"), MovementSpeed));

	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString::Printf(TEXT("MaxWalkSpeed : %f"), OwnerCharacterMovement->MaxWalkSpeed));

	GEngine->AddOnScreenDebugMessage(5, 3, FColor::Green, FString::Printf(TEXT("MovementElapsedTime : %f"), MovementElapsedTime));

	GEngine->AddOnScreenDebugMessage(4, 3, FColor::Green, FString::Printf(TEXT("GetLastMovementInputVector : %f, %f, %f"), OwnerCharacter->GetLastMovementInputVector().X, OwnerCharacter->GetLastMovementInputVector().Y, OwnerCharacter->GetLastMovementInputVector().Z));

	GEngine->AddOnScreenDebugMessage(13, 3, FColor::Green, FString::Printf(TEXT("CharacterVelocity : %f, %f, %f"), OwnerCharacter->GetVelocity().X, OwnerCharacter->GetVelocity().Y, OwnerCharacter->GetVelocity().Z));

	GEngine->AddOnScreenDebugMessage(3, 3, FColor::Green, FString::Printf(TEXT("RotationRate : %f, %f, %f"), OwnerCharacterMovement->RotationRate.Pitch, OwnerCharacterMovement->RotationRate.Yaw, OwnerCharacterMovement->RotationRate.Roll));
}

void UPlayerAnimInstance::SetMaxSpeedAndPlayRate()
{
	float ClampValue = FMath::Clamp(GetCurveValue(FName("MoveData_Speed")), 50.0f, 1000.0f);
	PlayRate = UKismetMathLibrary::SafeDivide(MovementSpeed, ClampValue);
}

void UPlayerAnimInstance::SetRotationRate(float MinLocomotionValue, float MaxLocomotionValue)
{
	float ClampedRotValue = UKismetMathLibrary::MapRangeClamped(GetCurveValue(FName("Movement_Rotation")), 0.0f, 1.0f, MinLocomotionValue, MaxLocomotionValue);
	OwnerCharacterMovement->RotationRate = FRotator(0.0f, ClampedRotValue, 0.0f);
}

bool UPlayerAnimInstance::SetMovementDirection(float MinValue, float MaxValue, bool Mincluding, bool Maxcluding, float &Direction) const
{
	float MovementAngle = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation());
	
	float ClampAngle = FMath::Clamp(MovementAngle, -180.0f, 180.0f);

	bool FrontCondition = OwnerCharacter->GetLastMovementInputVector().X < 0.0f && ClampAngle == 180.0f;
	bool BackCondition = OwnerCharacter->GetLastMovementInputVector().X > 0.0f && ClampAngle == -180.0f;

	if (UKismetMathLibrary::InRange_FloatFloat(ClampAngle, MinValue, MaxValue, Mincluding, Maxcluding))
	{
		if (FrontCondition || BackCondition)
		{
			ClampAngle *= -1.0f;
		}
		
		Direction = ClampAngle;
		return true;
	}
	Direction = 0.0f;
	return false;
}

void UPlayerAnimInstance::OnEntryMoveStartState(const FAnimNode_StateMachine &Machine, int32 PrevStateIndex, int32 NextStateIndex)
{
	PreviousUnitVector = OwnerCharacter->GetLastMovementInputVector();

	if (SetMovementDirection(-180.0f, -135.0f, true, false, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartLeft_180, MovementAnimStruct.JogStartLeft_180);
		GEngine->AddOnScreenDebugMessage(11, 3, FColor::Green, FString::Printf(TEXT("MovementStartAngle : %f"), MovementStartAngle));
		return;
	}
	else if (SetMovementDirection(-135.0f, -45.0f, true, false, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartLeft_90, MovementAnimStruct.JogStartLeft_90);
		GEngine->AddOnScreenDebugMessage(11, 3, FColor::Green, FString::Printf(TEXT("MovementStartAngle : %f"), MovementStartAngle));
		return;
	}
	else if (SetMovementDirection(45.0f, 135.0f, false, true, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartRight_90, MovementAnimStruct.JogStartRight_90);
		GEngine->AddOnScreenDebugMessage(11, 3, FColor::Green, FString::Printf(TEXT("MovementStartAngle : %f"), MovementStartAngle));
		return;
	}
	else if (SetMovementDirection(135.0f, 180.0f, false, true, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartRight_180, MovementAnimStruct.JogStartRight_180);
		GEngine->AddOnScreenDebugMessage(11, 3, FColor::Green, FString::Printf(TEXT("MovementStartAngle : %f"), MovementStartAngle));
		return;
	}
	else
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartForward, MovementAnimStruct.JogStartForward);
		GEngine->AddOnScreenDebugMessage(11, 3, FColor::Green, FString::Printf(TEXT("MovementStartAngle : %f"), MovementStartAngle));
		return;
	}
}

void UPlayerAnimInstance::OnEntryMoveStopState(const struct FAnimNode_StateMachine &Machine, int32 PrevStateIndex, int32 NextStateIndex)
{
	if (bIsJog)
	{
		DesiredMoveEndAnim = MovementAnimStruct.JogEnd;
	}
	if (!bIsJog)
	{
		DesiredMoveEndAnim = MovementAnimStruct.WalkEnd;
	}
}

void UPlayerAnimInstance::OnEntryPivotTurnState(const FAnimNode_StateMachine &Machine, int32 PrevStateIndex, int32 NextStateIndex)
{
	PreviousUnitVector = OwnerCharacter->GetLastMovementInputVector();

	if (UKismetMathLibrary::InRange_FloatFloat(PivotTurnAngle, -180, -135, true, false))
	{
		DesiredPivotTurnAnim = MovementAnimStruct.PivotTurn_Left180;
		return;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(PivotTurnAngle, -135, -45, true, false))
	{
		DesiredPivotTurnAnim = MovementAnimStruct.PivotTurn_Left90;
		return;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(PivotTurnAngle, 45, 135, false, true))
	{
		DesiredPivotTurnAnim = MovementAnimStruct.PivotTurn_Right90;
		return;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(PivotTurnAngle, 135, 180, false, true))
	{
		DesiredPivotTurnAnim = MovementAnimStruct.PivotTurn_Right180;
		return;
	}
	return;
}

void UPlayerAnimInstance::CheckCurrentDirection()
{
	if (OwnerCharacter->GetLastMovementInputVector() != FVector::ZeroVector)
	{
		if (PreviousUnitVector != OwnerCharacter->GetLastMovementInputVector())
		{
			bIsTurn = true;
			PreviousUnitVector = OwnerCharacter->GetLastMovementInputVector();

			float ReDirection = FMath::Clamp(CalculateDirection(OwnerCharacter->GetLastMovementInputVector(), OwnerCharacter->GetActorRotation()), -180, 180);
			PivotTurnAngle = ReDirection;

			GEngine->AddOnScreenDebugMessage(12, 3, FColor::Green, FString::Printf(TEXT("ReDirection : %f"), ReDirection));
			GEngine->AddOnScreenDebugMessage(9, 3, FColor::Green, FString("Turn"));
			
			GEngine->AddOnScreenDebugMessage(10, 3, FColor::Green, FString::Printf(TEXT("PreviousVector : %f, %f, %f"), PreviousUnitVector.X, PreviousUnitVector.Y, PreviousUnitVector.Z));
		}
	}
}

void UPlayerAnimInstance::DesiredStartMoveAnim(UAnimSequenceBase *DesiredWalkAnim, UAnimSequenceBase *DesiredJogAnim)
{
	if (bIsJog)
	{
		DesiredMoveStartAnim = DesiredJogAnim;
	}
	if (!bIsJog)
	{
		DesiredMoveStartAnim = DesiredWalkAnim;
	}
}

void UPlayerAnimInstance::FootIK(float DeltaSecond)
{
	if (!bIsInAir)
	{
		float Foot_L = CapsuleDistance("ik_foot_l");
		float Foot_R = CapsuleDistance("ik_foot_r");

		if (Foot_L != 0.0f || Foot_R != 0.0f)
		{
			const float PelvisTargetDisitance = Foot_L >= Foot_R ? Foot_L : Foot_R;

			Pelvis = FMath::FInterpTo(Pelvis, (PelvisTargetDisitance - 100.0f) * -1.0f, DeltaSecond, CurrentInterpSpeed);

			TTuple<float, FVector> FootTrace_L = FootTrace("ik_foot_l");
			TTuple<float, FVector> FootTrace_R = FootTrace("ik_foot_r");

			const float Distance_L = FootTrace_L.Get<0>();
			const FVector FootLVector = FootTrace_L.Get<1>();
			const FRotator MakeLRot(UKismetMathLibrary::DegAtan2(FootLVector.X, FootLVector.Z) * -1.0f, 0.0, UKismetMathLibrary::DegAtan2(FootLVector.Y, FootLVector.Z));

			L_FootIK = FMath::FInterpTo(L_FootIK, (Distance_L - 110.0f) / -45.0f, DeltaSecond, CurrentInterpSpeed);
			L_FootRotation = FMath::RInterpTo(L_FootRotation, MakeLRot, DeltaSecond, CurrentInterpSpeed);

			const float Distance_R = FootTrace_R.Get<0>();
			const FVector FootRVector = FootTrace_R.Get<1>();
			const FRotator MakeRRot(UKismetMathLibrary::DegAtan2(FootRVector.X, FootRVector.Z) * -1.0f, 0.0f, UKismetMathLibrary::DegAtan2(FootRVector.Y, FootRVector.Z));

			R_FootIK = FMath::FInterpTo(R_FootIK, (Distance_R - 110.0f) / -45.0f, DeltaSecond, CurrentInterpSpeed);
			R_FootRotation = FMath::RInterpTo(R_FootRotation, MakeRRot, DeltaSecond, CurrentInterpSpeed);
		}
	}
	else
	{
		L_FootIK = FMath::FInterpTo(L_FootIK, 0.0f, DeltaSecond, CurrentInterpSpeed);
		L_FootRotation = FMath::RInterpTo(L_FootRotation, FRotator::ZeroRotator, DeltaSecond, CurrentInterpSpeed);

		R_FootIK = FMath::FInterpTo(R_FootIK, 0.0f, DeltaSecond, CurrentInterpSpeed);
		R_FootRotation = FMath::RInterpTo(R_FootRotation, FRotator::ZeroRotator, DeltaSecond, CurrentInterpSpeed);
	}
}

float UPlayerAnimInstance::CapsuleDistance(FName SocketName)
{
	FVector MeshLocation = OwnerCharacter->GetMesh()->GetComponentLocation();
	FVector PelvisLocation = MeshLocation + FVector(0.0f, 0.0f, 98.0f);

	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(SocketName);
	FVector StartPos = FVector(SocketLocation.X, SocketLocation.Y, PelvisLocation.Z);
	FVector EndPos = FVector(StartPos - FVector(0.0f, 0.0f, 151.0f));

	FHitResult PelvisHit;
	FCollisionQueryParams PelvisCollisionParams;
	PelvisCollisionParams.AddIgnoredActor(OwnerCharacter);


	if (GetWorld()->LineTraceSingleByChannel(PelvisHit, StartPos, EndPos, ECollisionChannel::ECC_Visibility, PelvisCollisionParams))
	{
		return PelvisHit.Distance;
	}

	return 0.0f;
}

TTuple<float, FVector> UPlayerAnimInstance::FootTrace(FName SocketName)
{
	const FVector FootLocation = OwnerCharacter->GetMesh()->GetSocketLocation(SocketName);
	const FVector RootBoneLocation = OwnerCharacter->GetMesh()->GetSocketLocation("root");

	FHitResult IKHit;
	FVector StartPos = FVector(FootLocation.X, FootLocation.Y, RootBoneLocation.Z + 105.0f);
	FVector EndPos = StartPos - FVector(0.0f, 0.0f, 105.0f);
	FCollisionQueryParams IKColParam;
	IKColParam.AddIgnoredActor(OwnerCharacter);

	if (GetWorld()->LineTraceSingleByChannel(IKHit, StartPos, EndPos, ECollisionChannel::ECC_Visibility, IKColParam))
	{
		return MakeTuple(IKHit.Distance, IKHit.Normal);
	}

	return MakeTuple(999.0f, FVector::ZeroVector);
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