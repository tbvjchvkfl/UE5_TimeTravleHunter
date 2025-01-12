// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Character/AnimInstance/PlayerAnimInstance.h"
#include "Character/Controller/PlayerCharacterController.h"
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

	AddNativeStateEntryBinding(TEXT("KatanaState"), TEXT("Move_Start"), FOnGraphStateChanged::CreateUObject(this, &UPlayerAnimInstance::OnEntryWeaponStartState));

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
		if (OwnerController->bIsBasicAttack)
		{
			GEngine->AddOnScreenDebugMessage(0, 10, FColor::Green, FString("bIsBasicAttack : True"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 10, FColor::Green, FString("bIsBasicAttack : False"));
		}
		GEngine->AddOnScreenDebugMessage(1, 10, FColor::Green, FString::Printf(TEXT("BasicAttackIndex : %d"), BasicAttackIndex));
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
	BasicAttackIndex = 0;
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

	bIsKatana = OwnerController->bIsKatanaState;
	bIsSpear = OwnerController->bIsSpearState;
	bIsBow = OwnerController->bIsBowState;

	if (bIsKatana || bIsSpear || bIsBow)
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		OwnerCharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}
	if (!bIsKatana && !bIsSpear && !bIsBow)
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
		OwnerCharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = false;
	}

	MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity()), OwnerCharacter->GetBaseAimRotation()).Yaw;

	SetMaxSpeedAndPlayRate();
	CheckCurrentDirection();
	SetRotationRate(0.0f, 2000.0f);
}

void UPlayerAnimInstance::SetMaxSpeedAndPlayRate()
{
	OwnerCharacterMovement->MaxWalkSpeed = GetCurveValue(FName("Movement_Speed"));
	if (bIsJog)
	{
		float ClampValue = FMath::Clamp(GetCurveValue(FName("MoveData_Speed")), 450.0f, 1000.0f);
		PlayRate = UKismetMathLibrary::SafeDivide(500.0f, ClampValue);
	}
	else
	{
		float ClampValue = FMath::Clamp(GetCurveValue(FName("MoveData_Speed")), 100.0f, 1000.0f);
		PlayRate = UKismetMathLibrary::SafeDivide(150.0f, ClampValue);
	}
}

void UPlayerAnimInstance::SetRotationRate(float MinLocomotionValue, float MaxLocomotionValue)
{
	float ClampedRotValue = UKismetMathLibrary::MapRangeClamped(GetCurveValue(FName("Movement_Rotation")), 0.0f, 4.0f, MinLocomotionValue, MaxLocomotionValue);
	OwnerCharacterMovement->RotationRate = FRotator(0.0f, ClampedRotValue, 0.0f);
}

bool UPlayerAnimInstance::SetMovementDirection(float MinValue, float MaxValue, bool Mincluding, bool Maxcluding, float &Direction) const
{
	float MovementAngle = CalculateDirection(OwnerCharacter->GetLastMovementInputVector(), OwnerCharacter->GetActorRotation());
	
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
	if (SetMovementDirection(-180.0f, -135.0f, true, false, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartLeft_180, MovementAnimStruct.JogStartLeft_180);
		return;
	}
	else if (SetMovementDirection(-135.0f, -45.0f, true, false, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartLeft_90, MovementAnimStruct.JogStartLeft_90);
		return;
	}
	else if (SetMovementDirection(45.0f, 135.0f, false, true, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartRight_90, MovementAnimStruct.JogStartRight_90);
		return;
	}
	else if (SetMovementDirection(135.0f, 180.0f, false, true, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartRight_180, MovementAnimStruct.JogStartRight_180);
		return;
	}
	else
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartForward, MovementAnimStruct.JogStartForward);
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
	if (SetMovementDirection(-180.0f, -135.0f, true, false, MovementStartAngle))
	{
		DesiredPivotTurnAnim = MovementAnimStruct.PivotTurn_Left180;
		return;
	}
	else if (SetMovementDirection(-135.0f, -45.0f, true, false, MovementStartAngle))
	{
		DesiredPivotTurnAnim = MovementAnimStruct.PivotTurn_Left90;
		return;
	}
	else if (SetMovementDirection(45.0f, 135.0f, false, true, MovementStartAngle))
	{
		DesiredPivotTurnAnim = MovementAnimStruct.PivotTurn_Right90;
		return;
	}
	else if (SetMovementDirection(135.0f, 180.0f, false, true, MovementStartAngle))
	{
		DesiredPivotTurnAnim = MovementAnimStruct.PivotTurn_Right180;
		return;
	}
}

void UPlayerAnimInstance::OnEntryWeaponStartState(const FAnimNode_StateMachine &Machine, int32 PrevStateIndex, int32 NextStateIndex)
{
	if (SetMovementDirection(-180.0f, -135.0f, true, false, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartLeft_180, MovementAnimStruct.JogStartLeft_180);
		return;
	}
	else if (SetMovementDirection(-135.0f, -45.0f, true, false, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartLeft_90, MovementAnimStruct.JogStartLeft_90);
		return;
	}
	else if (SetMovementDirection(45.0f, 135.0f, false, true, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartRight_90, MovementAnimStruct.JogStartRight_90);
		return;
	}
	else if (SetMovementDirection(135.0f, 180.0f, false, true, MovementStartAngle))
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartRight_180, MovementAnimStruct.JogStartRight_180);
		return;
	}
	else
	{
		DesiredStartMoveAnim(MovementAnimStruct.WalkStartForward, MovementAnimStruct.JogStartForward);
		return;
	}
}

void UPlayerAnimInstance::CheckCurrentDirection()
{
	if (OwnerCharacter->GetLastMovementInputVector() != FVector::ZeroVector)
	{
		if (PreviousUnitVector != OwnerCharacter->GetLastMovementInputVector())
		{
			float ReDirection = FMath::Clamp(CalculateDirection(OwnerCharacter->GetLastMovementInputVector(), OwnerCharacter->GetActorRotation()), -180, 180);

			PivotTurnAngle = ReDirection;
			PreviousUnitVector = OwnerCharacter->GetLastMovementInputVector();

			if (UKismetMathLibrary::InRange_FloatFloat(PivotTurnAngle, -45, 45, true, true))
			{
				return;
			}
			bIsTurn = true;
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

void UPlayerAnimInstance::PlayEquipKatana()
{
	if (EquipKatana)
	{
		Montage_Play(EquipKatana, 1.2f);
	}
}

void UPlayerAnimInstance::PlayBasicAttack()
{
	if (!BasicKatanaAnimArray.IsEmpty())
	{
		OwnerController->bIsBasicAttack = true;
		switch (BasicAttackIndex)
		{
			case 0:
			{
				PlayFirstAttack();
				ResetBasicAttack(0);
			}
			break;
			case 1:
			{
				PlayMiddleAttack();
				ResetBasicAttack(1);
			}
			break;
			case 2:
			{
				PlayLastAttack();
				ResetBasicAttack(2);
			}
			break;
		}
	}
}

void UPlayerAnimInstance::PlayFirstAttack()
{
	if (BasicAttackIndex == 0)
	{
		if (OwnerController->bIsSpecialAttack && Montage_IsPlaying(SpecialKatanaAnimArray[0]))
		{
			Montage_Stop(0.2f, SpecialKatanaAnimArray[0]);
			Montage_Play(CombineAttackStruct.CombineSpecialKatanaAttack_3, 1.0f);
			BasicAttackIndex = 0;
		}
		if (OwnerController->bIsSpecialAttack && Montage_IsPlaying(SpecialKatanaAnimArray[1]))
		{
			Montage_Stop(0.2f, SpecialKatanaAnimArray[1]);
			Montage_Play(CombineAttackStruct.CombineSpecialKatanaAttack_4, 1.0f);
			BasicAttackIndex = 0;
		}
		if (!Montage_IsPlaying(BasicKatanaAnimArray[0]) && !OwnerController->bIsSpecialAttack)
		{
			Montage_Play(BasicKatanaAnimArray[0], 1.0f);
			BasicAttackIndex++;
		}
	}
}

void UPlayerAnimInstance::PlayMiddleAttack()
{
	if (BasicAttackIndex == 1)
	{
		if (Montage_GetPosition(BasicKatanaAnimArray[0]) > 0.6f)
		{
			Montage_Stop(0.2f, BasicKatanaAnimArray[0]);
			Montage_Play(BasicKatanaAnimArray[1], 1.0f);
			BasicAttackIndex++;
		}
	}
}

void UPlayerAnimInstance::PlayLastAttack()
{
	if (BasicAttackIndex == 2)
	{
		if (Montage_GetPosition(BasicKatanaAnimArray[1]) > 0.6f)
		{
			Montage_Stop(0.2f, BasicKatanaAnimArray[1]);
			Montage_Play(BasicKatanaAnimArray[2], 1.0f);
			BasicAttackIndex = 0;
		}
	}
}

void UPlayerAnimInstance::ResetBasicAttack(int32 AnimIndex)
{
	if (Montage_IsPlaying(BasicKatanaAnimArray[AnimIndex]))
	{
		GetWorld()->GetTimerManager().SetTimer(BasicAttackTimer, [this]()
			{
				OwnerController->bIsBasicAttack = false;
				OwnerController->bIsSpecialAttack = false;
				if (BasicAttackIndex == 1 || BasicAttackIndex == 2)
				{
					if (!Montage_IsPlaying(BasicKatanaAnimArray[0]) && !Montage_IsPlaying(BasicKatanaAnimArray[1]))
					{
						BasicAttackIndex = 0;
					}
				}
				GetWorld()->GetTimerManager().ClearTimer(BasicAttackTimer);
			}, 
			BasicKatanaAnimArray[AnimIndex]->GetPlayLength(), false);
	}
}

void UPlayerAnimInstance::SpecialAttackHold()
{
	if (Hold_Anim)
	{
		Montage_Play(Hold_Anim, 0.5f);
	}
}

void UPlayerAnimInstance::PlaySpecialAttack(float ButtonElapsedTime)
{
	OwnerController->bIsSpecialAttack = true;
	if (ButtonElapsedTime < 1.0f)
	{
		if (OwnerController->bIsBasicAttack && BasicAttackIndex == 1)
		{
			if (Montage_IsPlaying(BasicKatanaAnimArray[0]))
			{
				Montage_Stop(0.2f, BasicKatanaAnimArray[0]);
				Montage_Stop(0.2f, SpecialKatanaAnimArray[0]);
				Montage_Play(CombineAttackStruct.CombineSpecialKatanaAttack_0, 1.0f);
			}
		}
		else if (OwnerController->bIsBasicAttack && BasicAttackIndex == 2)
		{
			if (Montage_IsPlaying(BasicKatanaAnimArray[1]))
			{
				Montage_Stop(0.2f, BasicKatanaAnimArray[1]);
				Montage_Stop(0.2f, SpecialKatanaAnimArray[0]);
				Montage_Play(CombineAttackStruct.CombineSpecialKatanaAttack_1, 1.0f);
			}
		}
		else if (OwnerController->bIsBasicAttack && BasicAttackIndex == 0)
		{
			if (Montage_IsPlaying(BasicKatanaAnimArray[2]))
			{
				Montage_Stop(0.2f, BasicKatanaAnimArray[2]);
				Montage_Stop(0.2f, SpecialKatanaAnimArray[0]);
				Montage_Play(CombineAttackStruct.CombineSpecialKatanaAttack_2, 1.0f);
			}
		}
		else
		{
			Montage_Play(SpecialKatanaAnimArray[0], 1.0f);
		}
		ResetSpecialAttack(0);
	}
	if (ButtonElapsedTime >= 1.0f && ButtonElapsedTime < 2.0f)
	{
		Montage_Play(SpecialKatanaAnimArray[1], 1.0f);
		ResetSpecialAttack(1);
	}
	if (ButtonElapsedTime >= 2.0f)
	{
		Montage_Play(SpecialKatanaAnimArray[2], 1.0f);
		ResetSpecialAttack(2);
	}
}

void UPlayerAnimInstance::ResetSpecialAttack(int32 AnimIndex)
{
	GetWorld()->GetTimerManager().SetTimer(BasicAttackTimer, [this]()
		{
			OwnerController->bIsBasicAttack = false;
			OwnerController->bIsSpecialAttack = false;
			BasicAttackIndex = 0;
			GetWorld()->GetTimerManager().ClearTimer(BasicAttackTimer);
		},
		SpecialKatanaAnimArray[AnimIndex]->GetPlayLength(), false);
}
