
PlayerCharacter
-

Contents
-
- Advanced Locomotion
- Traversal (Vaulting, Hurdling, Mantling), Assasination With MotionWarping
- RagDoll System
- Combat Combo Action
- FootIK


예시 영상
-

핵심 코드
-

> - ### Advanced Locomotion ###
> - AddNativeStateEntryBinding()함수를 사용해서 스테이트 머신의 특정 상태에 진입하면 SetMovementDirection()함수가 호출 되게 하여 각 방향에 따른 MoveStart애니메이션이 출력될 수 있게 하였습니다.
> - 이 후, 애니메이션 시퀀스 내에 커브 값을 사용해서 각 애니메이션별 이동 속도와 회전 속도를 조절해 주었습니다.
<pre>
  <code>
    void UPlayerAnimInstance::NativeInitializeAnimation()
    {
    	Super::NativeInitializeAnimation();
    	AddNativeStateEntryBinding(TEXT("LocomotionState"), TEXT("Move_Start"), FOnGraphStateChanged::CreateUObject(this, &UPlayerAnimInstance::OnEntryMoveStartState));
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
  </code>
</pre>

> - ### FootIK ###
> - 라인트레이스를 이용하여 PlayerCharacter의 골반에서 바닥까지의 거리와 발에서 바닥까지의 거리를 구해주었고, 블루프린트의 Transform Bone노드와 LegIK노드를 사용하여 골반과 발의 위치를 조정해주었습니다.
> - FMath::FInterpTo함수를 사용하여 골반과 발의 위치가 부드럽게 변경될 수 있도록 보간해주었습니다.
> - 경사로에 서있을 때, 발이 땅에 묻히는 현상을 해결하기 위해 UKismetMathLibrary::DegAtan2함수를 사용하여 회전해야하는 각도를 계산해주었고, RInterpTo함수를 사용하여 보간해주었습니다.
<pre>
  <code>
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
  </code>
</pre>

> - ### Traversal System ###
> - 라인 트레이스를 이용하여 파쿠르가 가능한 벽이라면 벽의 높이를 체크한 후 Vaulting과 Climbing으로 나누어 몽타주가 실행되게 해주었습니다.
> - Vaulting은 앉은 상태와 서 있는 상태로 나누어 서로 다른 몽타주를 실행하게 해주었고, 앉은 상태의 Vaulting은 MotionWraping을 사용하여 지정된 위치로 이동하게 만들었습니다.
<pre>
  <Code>
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
    		if (AParkourWall *ParkourWall = Cast<AParkourWall>(DetectingWallHit.GetActor()))
    		{
    			if (ParkourWall->GetWallSize().Z >= 2.0f)
    			{
    				FHitResult MantleLandingHit;
    				FVector MantleStartPos = DetectingWallHit.Location + GetActorRotation().Vector() * 10.0f + FVector(0.0f, 0.0f, 1000.0f);
    				FVector MantleEndPos = MantleStartPos - FVector(0.0f, 0.0f, 1000.0f);
    
    				if (GetWorld()->LineTraceSingleByChannel(MantleLandingHit, MantleStartPos, MantleEndPos, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams))
    				{
    					MantlePos = MantleLandingHit.Location;
    					Climbing();
    				}
    				DrawDebugLine(GetWorld(), MantleStartPos, MantleEndPos, FColor::Cyan, false, 3.0f);
    			}
    			else
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
    		}
    	}
    }
  </Code>
</pre>

> - ### Assasination System (With. MotionWarping Component)
> - 적의 등 뒤로 SphereComponent를 배치하여 Player가 쏜 LineTrace가 SphereComponent에 닿으면 애니메이션 몽타주가 실행되게 해주었습니다.
> - MotionWraping Component를 사용하여 Player와 Enemy사이에 거리가 조금 떨어져 있거나 틀어져 있더라도, 지정된 위치로 이동하여 몽타주가 실행될 수 있게 해주었습니다.
> - Player와 Enemy의 몽타주 실행이 종료되면 Enemy의 SetSimulatePhysics를 true로 바꿔주어 Ragdoll 상태가 되도록 구현해주었습니다.
<pre>
  <code>
    = PlayerCharacter.cpp =
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

    = EnemyCharacter.cpp =
    void AEnemyCharacter::StealthAssain(FVector &Location, FRotator &Rotation)
    {
    	EOwningAnimInstance->PlayAssasination_Vic();
    	GetWorld()->GetTimerManager().SetTimer(RagDollTimer, this, &AEnemyCharacter::RagDoll, EOwningAnimInstance->Assasination_Vic->GetPlayLength(), false);
    	Location = AssasinationPosMesh->GetComponentLocation();
    	Rotation = GetActorRotation();
    }
    
    void AEnemyCharacter::RagDoll()
    {
    	GetMesh()->SetSimulatePhysics(true);
    }
  </code>
</pre>

> - ### Combat Combo Action ###
> - BasicAttackIndex를 만들어 BasicAttack의 Input이 입력될 때 마다 다른 애니메이션이 실행될 수 있게 해주었습니다.
> - BasicAttack 액션이 실행될 때 마다 ResetBasicAttack함수를 실행해주어 BasicAttack이 최종 액션까지 모두 실행되지 않으면 다시 처음 액션부터 실행될 수 있도록 해주었습니다.
> - SpecialAttack의 경우 FInputActionInstance의 GetElapsedTime() 함수를 사용해서 버튼이 눌린 시간에 따라 서로 다른 액션을 실행하게 해주었습니다.
> - CombineAction의 경우 각 Attack 함수가 호출될 때 마다 true가 되는 bIsBasicAttack과 bIsSpecialAttack 변수와 BasicAttackIndex를 사용하여 입력 단계에 따른 키 조합 액션을 구현해주었습니다.
<pre>
  <code>
    
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

  </code>
</pre>



