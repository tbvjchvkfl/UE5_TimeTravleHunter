
PlayerCharacter
-

Contents
-
- Advanced Locomotion
- Traversal (Vaulting, Hurdling, Mantling)
- Combat Combo Action
- Assasination With MotionWarping
- RagDoll System

예시 영상
-

핵심 코드
-

> - ### Advanced Locomotion ###
> - AddNativeStateEntryBinding()함수를 사용해서 스테이트 머신의 특정 상태에 진입하면 SetMovementDirection함수가 호출 되게 하여 각 방향에 따른 MoveStart애니메이션이 출력될 수 있게 하였습니다.
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
> - 
> - 


> - ### Combat Combo Action ###
> -
> -


> - ### Assasination System (With. MotionWarping Component)
> - asdf
> - 
