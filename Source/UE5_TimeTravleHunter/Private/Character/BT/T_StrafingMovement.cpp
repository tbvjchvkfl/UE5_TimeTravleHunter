// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Character/BT/T_StrafingMovement.h"
#include "Character/Controller/EnemyCharacterController.h"
#include "Character/Enemy/EnemyCharacter.h"

// Engine
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UT_StrafingMovement::UT_StrafingMovement(FObjectInitializer const &ObjectInitializer)
{
	NodeName = "StrafingMovement";
}

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
