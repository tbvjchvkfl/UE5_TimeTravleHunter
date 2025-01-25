EnemyCharacter
-

코드 상세
-
 === 이미지 예시 ===


> - ### T_MoveToRandomLocation ###
> - UNavigationSystemV1 클래스를 이용하여 현재 월드에 있는 NaviMesh를 가져왔고, GetRandomPointInNavigableRadius함수를 사용하여 해당 노드가 실행될 때, 랜덤한 위치를 탐색하고 해당 탐색한 값을 블랙보드 키에 저장해주었습니다.
> - 이후, 시퀀스 노드와 MoveTo노드를 활용하여 블랙보드 내 값이 저장된 변수로 이동하게 구현했습니다.
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
  </code>
</pre>

> - ### T_MoveToTargetLocation ###
> - ExecuteTask함수에서 AIController::MoveToLocation함수를 사용하여 PathFollowing을 통해 PlayerCharacter의 위치로 이동하게 해주었고, 노드의 결과를 InProgress로 반환하여 대기 상태로 바꿔주었습니다.
> - 이후, AIController::OnMoveCompleted 함수에 캐릭터가 PathFollowing을 통해 이동이 완료되었다면 UBehaviorTreeComponent::GetActiveNode함수와 FinishLatentTask 함수를 통해 해당 노드의 상태를 Succeeded로 바꿔주었습니다.

<pre>
 <code>
   ========= T_MoveToTargetLocation.cpp =========
  
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
     
   ========= EnemyController.cpp =========
     
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

> - ### T_StrafingMovement ###
> - StrafeingMovement노드에서도 AIController::MoveToLocation 함수를 사용하였으나, MoveToLocation의 6번째 인자를 이용하여 측면이동을 할 수 있게 해주었습니다.
> - 그리고 AActor::SetFocus함수를 사용하여 EnemyCharacter의 시점을 PlayerCharacter에 고정시켜주었습니다.
> - 이후, FMath::FRand를 이용하여 0에서 1사이의 랜덤한 실수 값을 구하고 이를 정수 0과 1 중 하나로 변환하여 캐릭터가 좌, 우 랜덤하게 방향이동을 할 수 있도록 해주었습니다.

<pre>
 <code> 
     ========= T_StrafingMovement.cpp =========
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
      
      ========= EnemyCharacter.cpp =========
      
   FVector AEnemyCharacter::StrafeMovement()
   {
   	FVector DestLoc = FVector::ZeroVector;
   	float RandomValue = FMath::FRand();
   	int32 SelectedDirIndex = RandomValue < 0.5f ? 0 : 1;
   	FVector CharacterRightVector = GetActorRightVector() * 200.0f;
   	if (SelectedDirIndex == 0)
   	{
   		DestLoc = GetActorLocation() + CharacterRightVector;
   	}
   	if (SelectedDirIndex == 1)
   	{
   		DestLoc = GetActorLocation() - CharacterRightVector;
   	}
   	return DestLoc;
   }
     
   void AEnemyCharacter::ActiveStrafe(bool ActivationValue)
   {
   	if (OwningController)
   	{
   		if (ActivationValue)
   		{
   			OwningController->SetFocus(TargetCharacter);
   			OwningController->bIsStrafe = true;
   		}
   		else
   		{
   			OwningController->SetFocus(nullptr);
   			OwningController->bIsStrafe = false;
   		}
   	}
   }
      
      ========= EnemyController.cpp =========
     
   void AEnemyCharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult &Result)
   {
   	if (Result.IsSuccess())
   	{
   		if (auto StrafingMovement = Cast<UT_StrafingMovement>(EnemyTreeComponent->GetActiveNode()))
   		{
   			StrafingMovement->FinishLatentTask(*EnemyTreeComponent, EBTNodeResult::Succeeded);
   		}
   	}
   }

      
 </code>
</pre>
