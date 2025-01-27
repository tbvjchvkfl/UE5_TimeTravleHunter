// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Character/BT/T_MoveToRandomLocation.h"
#include "Character/Controller/EnemyCharacterController.h"
#include "Character/Enemy/EnemyCharacter.h"

// Engine
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UT_MoveToRandomLocation::UT_MoveToRandomLocation(FObjectInitializer const &ObjectInitializer)
{
	NodeName = "MoveToRandomLocation";
}

EBTNodeResult::Type UT_MoveToRandomLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	if (const auto Controller = Cast<AEnemyCharacterController>(OwnerComp.GetAIOwner()))
	{
		if (const auto PlayerCharacter = Cast<AEnemyCharacter>(Controller->GetOwningCharacter()))
		{
			UNavigationSystemV1 *NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			//UNavigationSystemV1::GetNavigationSystem(GetWorld());
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
