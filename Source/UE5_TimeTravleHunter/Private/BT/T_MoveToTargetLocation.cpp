// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Character/BT/T_MoveToTargetLocation.h"
#include "Character/Controller/EnemyCharacterController.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/Player/PlayerCharacter.h"


// Engine
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UT_MoveToTargetLocation::UT_MoveToTargetLocation(FObjectInitializer const &ObjectInitializer)
{
	NodeName = "MoveToTargetLocation";
}

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
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("PlayerLocation", PlayerCharacter->GetActorLocation());
			FVector DestinationPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("PlayerLocation");
			Controller->MoveToLocation(DestinationPos, AcceptableTargetRadius, true, true, false, false);
		}
	}
	return Result;
}

void UT_MoveToTargetLocation::ModifyNodeResult(const FPathFollowingResult &Result)
{
	if (Result.IsSuccess())
	{
		FinishLatentTask(*TreeComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(*TreeComp, EBTNodeResult::Failed);
	}
}
