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
<pre>
  <code>
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
> - asdf
> - 


> - ### Combat Combo Action ###
> -
> -


> - ### Assasination System (With. MotionWarping Component)
> - asdf
> - 
