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
      	}
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
