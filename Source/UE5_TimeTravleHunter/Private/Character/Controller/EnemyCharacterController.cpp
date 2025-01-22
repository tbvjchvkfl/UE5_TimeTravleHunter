// Fill out your copyright notice in the Description page of Project Settings.


//Game Framwork
#include "Character/Controller/EnemyCharacterController.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/AnimInstance/EnemyAnimInstance.h"

// Engine
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyCharacterController::AEnemyCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacterController::BeginPlay()
{
	Super::BeginPlay();
	SetUpBehaviorAndBoard();
}

void AEnemyCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacterController::OnPossess(APawn *InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn)
	{
		OwningCharacter = Cast<AEnemyCharacter>(InPawn);
		OwningInstance = Cast<UEnemyAnimInstance>(OwningCharacter->GetMesh()->GetAnimInstance());
	}
}

void AEnemyCharacterController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AEnemyCharacterController::SetUpBehaviorAndBoard()
{
	if (EnemyTree)
	{
		UBlackboardComponent *BlackBoardComponent;
		if (UseBlackboard(EnemyTree->BlackboardAsset, BlackBoardComponent))
		{
			RunBehaviorTree(EnemyTree);
		}
	}
}
