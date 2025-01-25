// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Character/BT/T_MeleeAttack.h"
#include "Character/Controller/EnemyCharacterController.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/AnimInstance/EnemyAnimInstance.h"

// Engine


UT_MeleeAttack::UT_MeleeAttack(FObjectInitializer const &ObjectInitializer)
{
	NodeName = "MeleeAttack";
}

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
