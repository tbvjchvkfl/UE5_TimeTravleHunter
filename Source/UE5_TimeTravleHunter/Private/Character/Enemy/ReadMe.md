EnemyCharacter
-

Contents
-
- BehaviorTree
- AI Controller
- EnemyCharacter

핵심 코드
-
 === 이미지 예시 ===


> - ### T_MoveToRandomLocation & T_MoveToTargetLocation ###
> - 
<pre>
  <code>
   =========== T_MoveToRandomLocation.cpp ===========
    
   EBTNodeResult::Type UT_MoveToRandomLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
   {
   	if (const auto Controller = Cast<AEnemyCharacterController>(OwnerComp.GetAIOwner()))
   	{
   		if (const auto PlayerCharacter = Cast<AEnemyCharacter>(Controller->GetOwningCharacter()))
   		{
   			UNavigationSystemV1 *NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
   			FNavLocation LocationInNaviMesh;
   			if (NavigationSystem->GetRandomPointInNavigableRadius(PlayerCharacter->GetActorLocation(), SearchRadius, LocationInNaviMesh))
   			{
   				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), LocationInNaviMesh.Location);
   				return EBTNodeResult::Succeeded;
   			}
   		}
   	}
   	return EBTNodeResult::Failed;
   }

            
    =========== UT_MoveToTargetLocation.cpp ===========
      
   EBTNodeResult::Type UT_MoveToTargetLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
   {
   	EBTNodeResult::Type Result = EBTNodeResult::Failed;
   	TreeComp = &OwnerComp;
   	if (auto Controller = Cast<AEnemyCharacterController>(OwnerComp.GetAIOwner()))
   	{
   		AEnemyCharacter *OwningCharacter = Controller->GetOwningCharacter();
   		APlayerCharacter *PlayerCharacter = Controller->GetTargetCharacter();
   		if (OwningCharacter && PlayerCharacter)
   		{
   			Result = EBTNodeResult::InProgress;
   			if (!Controller->bIsRun)
   			{
   				Controller->bIsRun = true;
   			}
   			OwnerComp.GetBlackboardComponent()->SetValueAsVector("PlayerLocation", PlayerCharacter->GetActorLocation());
   			FVector DestinationPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("PlayerLocation");
   			Controller->MoveToLocation(DestinationPos, AcceptableTargetRadius, true, true, false, false);
   		}
   	}
   	return Result;
   }
  </code>
</pre>

> - ### T_StrafingMovement ###
> - 
<pre>
 <code> 
   EBTNodeResult::Type UT_StrafingMovement::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
   {
   	EBTNodeResult::Type Result = EBTNodeResult::Failed;
   	if (auto Controller = Cast<AEnemyCharacterController>(OwnerComp.GetAIOwner()))
   	{
   		if (auto ECharacter = Cast<AEnemyCharacter>(Controller->GetOwningCharacter()))
   		{
   			Result = EBTNodeResult::InProgress;
   			if (Controller->bIsRun)
   			{
   				Controller->bIsRun = false;
   			}
   			ECharacter->ActiveStrafe(true);
   			OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetAroundLocation", ECharacter->StrafeMovement());
   
   			FVector DestinationPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("TargetAroundLocation");
   			Controller->MoveToLocation(DestinationPos, 10.0f, true, true, false, true);
   		}
   	}
   	return Result;
   }
 </code>
</pre>

> - ### T_MeleeAttack ###
> - 
<pre>
 <code>
   EBTNodeResult::Type UT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
   {
   	EBTNodeResult::Type Result = EBTNodeResult::Failed;
   	if (auto Controller = Cast<AEnemyCharacterController>(OwnerComp.GetAIOwner()))
   	{
   		if (auto ECharacter = Cast<AEnemyCharacter>(Controller->GetOwningCharacter()))
   		{
   			if (auto ECharacterAnimInstance = Cast<UEnemyAnimInstance>(ECharacter->GetMesh()->GetAnimInstance()))
   			{
   				Result = EBTNodeResult::InProgress;
   
   				ECharacterAnimInstance->PlayMeleeAttack();
   				GetWorld()->GetTimerManager().SetTimer(DesiredNodeResultTimer, [&]()
   					{
   						GetWorld()->GetTimerManager().ClearTimer(DesiredNodeResultTimer);
   						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
   					}, ECharacterAnimInstance->MeleeAttack_Anim->GetPlayLength(), false);
   			}
   		}
   	}
   	return Result;
   }
 </code>
</pre>

> - ### EnemyCharacterController ###
> - 
<pre>
 <code>
   void AEnemyCharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult &Result)
   {
   	if (Result.IsSuccess())
   	{
   		if (auto MoveTargetLocNode = Cast<UT_MoveToTargetLocation>(EnemyTreeComponent->GetActiveNode()))
   		{
   			MoveTargetLocNode->FinishLatentTask(*EnemyTreeComponent, EBTNodeResult::Succeeded);
   			bIsAttackReady = true;
   			GetBlackboardComponent()->SetValueAsBool("ReadyToAttack", bIsAttackReady);
   		}
   		if (auto StrafingMovement = Cast<UT_StrafingMovement>(EnemyTreeComponent->GetActiveNode()))
   		{
   			StrafingMovement->FinishLatentTask(*EnemyTreeComponent, EBTNodeResult::Succeeded);
   		}
   	}
   }
 </code>
</pre>
