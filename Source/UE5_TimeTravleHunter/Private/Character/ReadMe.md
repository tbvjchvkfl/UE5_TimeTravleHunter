
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

> - ### FootIK ###
> - 

