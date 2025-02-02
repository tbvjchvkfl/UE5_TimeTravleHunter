EnemyCharacter
-

핵심 코드
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

### Enemy Pool
> - 이미지 예시
> - 어느 레벨에서든 간단하게 배치할 수 있도록 AActor를 상속받아 만든 클래스에 TMap을 사용해서 EditEnemyPoolList와 EnemyPool을 만들어주었습니다.
> - EditEnemyPoolList는 게임 디자이너(게임 기획자)가 레벨에 배치할 Enemy클래스와 개수를 정할 수 있도록 TSubclassOf<AEnemyCharacter>와 int32를 Key와 Value로 선언했고, EnemyPool은 EditEnemyPoolList와 같은 타입의 Key와 실제 Spawn될 EnemyCharcter의 인스턴스들을 담을 배열을 구조체로 만들어 Value에 넣어 선언했습니다.

<pre>
 <code>
         ========= EnemyPool.h =========

    struct FEnemyPoolStruct
    {
    	TArray<AEnemyCharacter *> NPCList;
    	FEnemyPoolStruct()
    	{
    		NPCList.Empty();
    	}
    	void AddToNPCList(AEnemyCharacter *Character)
    	{
    		NPCList.Add(Character);
    	}
    	void RemoveToNPCList()
    	{
    		NPCList.Pop();
    	}
    	bool NPCListIsEmpty()
    	{
    		return NPCList.IsEmpty();
    	}
    };
      
   class UE5_TIMETRAVLEHUNTER_API AEnemyPool : public AActor
   {
    UPROPERTY(EditAnywhere, Category = "PoolDefault | Spawn Info")
   	TMap<TSubclassOf<AEnemyCharacter>, int32> EditEnemyPoolList;

   	UPROPERTY()
   	AEnemyCharacter *SpawnCharacterEnemy;
   
   	UPROPERTY()
   	TMap<TSubclassOf<AEnemyCharacter>, FEnemyPoolStruct> EnemyPool;
   };
 </code>
</pre>

> - EnemyPool의 BeginPlay함수에서 InitEnemyPool을 호출하여 PoolActor를 레벨에 배치한 후 게임이 시작되면 EditEnemyPoolList를 순회하며 정해진 개수 만큼 해당 클래스의 인스턴스를 만들어 배열에 담아주었습니다.
> - 각 EnemyCharacter의 인스턴스들은 배열에 담겨져있는 상태에선 콜리전을 끄고 Visibility를 Hidden으로 바꿔 게임에서 보이지 않는 상태로 만들어주었습니다.

<pre>
 <code>
         ========= EnemyPool.cpp =========

   void AEnemyPool::InitEnemyPool()
   {
   	if (!EditEnemyPoolList.IsEmpty())
   	{
   		for (auto EnemyElem : EditEnemyPoolList)
   		{
   			if (EnemyElem.Key)
   			{
   				EnemyPool.Add(EnemyElem.Key);
   				for (int32 i = 0; i < EnemyElem.Value; i++)
   				{
   					SpawnCharacterEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyElem.Key, GetActorLocation(), GetActorRotation());
   					if (SpawnCharacterEnemy)
   					{
   						SpawnCharacterEnemy->SetActorHiddenInGame(true);
   						SpawnCharacterEnemy->SetActorEnableCollision(false);
   						EnemyPool[EnemyElem.Key].AddToNPCList(SpawnCharacterEnemy);
   					}
   				}
   			}
   		}
   	}
   }
 </code>
</pre>

> - 레벨에 배치된 ActorPool에서 EnemyCharacter가 Spawn될 때에는 UseEnemyPool함수로 EnemyCharacter를 담고 있는 배열의 가장 마지막 Index부터 Visibiltiy와 콜리전을 모두 켠 상태에서 SpawnPoint로 넘겨주었고, 배열에서는 Pop하여 SpawnCount를 체크해주었습니다.

<pre>
 <code>
         ========= EnemyPool.cpp =========
   
   AEnemyCharacter *AEnemyPool::UseEnemyPool(TSubclassOf<AEnemyCharacter> CharacterClass)
   {
   	AEnemyCharacter *ECharacter{};
   	for (auto EnemyElem : EnemyPool)
   	{
   		if (EnemyElem.Key == CharacterClass)
   		{
   			if (!EnemyElem.Value.NPCListIsEmpty())
   			{
   				ECharacter = EnemyElem.Value.NPCList[EnemyElem.Value.NPCList.Num() - 1];
   				ECharacter->SetActorHiddenInGame(false);
   				ECharacter->SetActorEnableCollision(true);
   				EnemyElem.Value.RemoveToNPCList();
   			}
   		}
   	}
   	return ECharacter;
   }

    ========= SpawnPoint.cpp =========
   void ASpawnPoint::SpawnObject()
   {
   	if (ItemSpawnType == ESpawnType::EnemyCharacter)
   	{
   		if (EnemyPoolClass)
   		{
   			TArray<AActor *>ActorArray;
   			UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyPoolClass, ActorArray);
   			for (auto ActorElem : ActorArray)
   			{
   				if (ActorElem)
   				{
   					EnemyPool = Cast<AEnemyPool>(ActorElem);
   					break;
   				}
   			}
   			if (EnemyPool)
   			{
   				if (SpawnCharacterClass && !EnemyPool->CheckEnemyPoolIsEmpty(SpawnCharacterClass))
   				{
   					SpawnCharacter = EnemyPool->UseEnemyPool(SpawnCharacterClass);
   					SpawnCharacter->SetActorLocation(GetActorLocation());
   					this->Destroy();
   				}
   			}
   		}
   	}
   }
 </code>
</pre>

> - 레벨에 배치된 ActorPool에서 EnemyCharacter가 Spawn될 때에는 UseEnemyPool함수로 EnemyCharacter를 담고 있는 배열의 가장 마지막 Index부터 Visibiltiy와 콜리전을 모두 켠 상태에서 SpawnPoint로 넘겨주었고, 배열에서는 Pop하여 SpawnCount를 체크해주었습니다.
> - SpawnPoint에서 EnemyCharacter를 Spawn할 때에는 아래 SpawnObject함수를 BeginPlay에서 호출해 주었는데, 실제 개발 과정에서 Pool에 있는 개수보다 SpawnPoint가 많아지는 경우를 고려하여 CheckEnemyPoolIsEmpty함수를 통해 EnemyCharacter를 담고 있는 배열이 비어있는게 아니라면 Spawn할 수 있도록 구현했습니다.

<pre>
 <code>
         ========= EnemyPool.cpp =========
   
   AEnemyCharacter *AEnemyPool::UseEnemyPool(TSubclassOf<AEnemyCharacter> CharacterClass)
   {
   	AEnemyCharacter *ECharacter{};
   	for (auto EnemyElem : EnemyPool)
   	{
   		if (EnemyElem.Key == CharacterClass)
   		{
   			if (!EnemyElem.Value.NPCListIsEmpty())
   			{
   				ECharacter = EnemyElem.Value.NPCList[EnemyElem.Value.NPCList.Num() - 1];
   				ECharacter->SetActorHiddenInGame(false);
   				ECharacter->SetActorEnableCollision(true);
   				EnemyElem.Value.RemoveToNPCList();
   			}
   		}
   	}
   	return ECharacter;
   }
    
   bool AEnemyPool::CheckEnemyPoolIsEmpty(TSubclassOf<AEnemyCharacter> CharacterClass)
   {
   	bool ReturnValue = false;
   	for (auto EnemyElem : EnemyPool)
   	{
   		if (CharacterClass == EnemyElem.Key)
   		{
   			if (EnemyElem.Value.NPCListIsEmpty())
   			{
   				ReturnValue = true;
   			}
   		}
   	}
   	return ReturnValue;
   }
    
    ========= SpawnPoint.cpp =========
   void ASpawnPoint::SpawnObject()
   {
   	if (ItemSpawnType == ESpawnType::EnemyCharacter)
   	{
   		if (EnemyPoolClass)
   		{
   			TArray<AActor *>ActorArray;
   			UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyPoolClass, ActorArray);
   			for (auto ActorElem : ActorArray)
   			{
   				if (ActorElem)
   				{
   					EnemyPool = Cast<AEnemyPool>(ActorElem);
   					break;
   				}
   			}
   			if (EnemyPool)
   			{
   				if (SpawnCharacterClass && !EnemyPool->CheckEnemyPoolIsEmpty(SpawnCharacterClass))
   				{
   					SpawnCharacter = EnemyPool->UseEnemyPool(SpawnCharacterClass);
   					SpawnCharacter->SetActorLocation(GetActorLocation());
   					this->Destroy();
   				}
   			}
   		}
   	}
   }
 </code>
</pre>

> - 마지막으로 레벨에 배치된 EnemyCharacter가 게임 상에서 죽었을 때, 해당 캐릭터의 Visibility와 콜리전을 배열에 담겨져 있던 초기 상태로 되돌린 후, EnemyPool에 ReturnEnemyPool함수를 호출하여 재사용할 수 있도록 해주었습니다.

<pre>
 <code>
    ========= EnemyPool.cpp =========
   void AEnemyPool::ReturnEnemyPool(TSubclassOf<AEnemyCharacter> CharacterClass, AEnemyCharacter *Character)
   {
   	for (auto EnemyElem : EnemyPool)
   	{
   		if (EnemyElem.Key == CharacterClass)
   		{
   			EnemyElem.Value.AddToNPCList(Character);
   		}
   	}
   }
    
    ========= EnemyCharacter.cpp =========
   void AEnemyCharacter::RagDoll()
   {
   	GetMesh()->SetSimulatePhysics(true);
   	FTimerHandle DeathTimer;
   	GetWorld()->GetTimerManager().SetTimer(DeathTimer, [this]() {
   		SetActorEnableCollision(false);
   		SetActorHiddenInGame(true);
   		TArray<AActor *>ActorArray;
   		UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyPoolClass, ActorArray);
   		for (auto ActorElem : ActorArray)
   		{
   			if (ActorElem)
   			{
   				EnemyPool = Cast<AEnemyPool>(ActorElem);
   				break;
   			}
   		}
   		if (EnemyPool)
   		{
   			EnemyPool->ReturnEnemyPool(PoolKey, this);
   		}
   		}, 5.0f, false);
   }
    
 </code>
</pre>
