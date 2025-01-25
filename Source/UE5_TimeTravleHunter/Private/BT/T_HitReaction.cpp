// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Character/BT/T_HitReaction.h"
#include "Character/Controller/EnemyCharacterController.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/AnimInstance/EnemyAnimInstance.h"

// Engine


UT_HitReaction::UT_HitReaction(FObjectInitializer const &ObjectInitializer)
{
	NodeName = "HitReaction";
}

EBTNodeResult::Type UT_HitReaction::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	if (auto Controller = Cast<AEnemyCharacterController>(OwnerComp.GetAIOwner()))
	{
		if (auto ECharacter = Cast<AEnemyCharacter>(Controller->GetOwningCharacter()))
		{
			if (auto ECharacterAnimInstance = Cast<UEnemyAnimInstance>(ECharacter->GetMesh()->GetAnimInstance()))
			{
				Result = EBTNodeResult::InProgress;

				ECharacterAnimInstance->PlayHitReaction();

				if (Controller->bIsHit_Tick)
				{
					GetWorld()->GetTimerManager().SetTimer(DesiredNodeResultTimer, [&]()
						{
							GetWorld()->GetTimerManager().ClearTimer(DesiredNodeResultTimer);
							FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						}, ECharacterAnimInstance->HitReaction_Tick_Anim->GetPlayLength(), false);
				}
				if (Controller->bIsHit_KnockBack)
				{
					GetWorld()->GetTimerManager().SetTimer(DesiredNodeResultTimer, [&]()
						{
							GetWorld()->GetTimerManager().ClearTimer(DesiredNodeResultTimer);
							FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						}, ECharacterAnimInstance->HitReaction_KnockBack_Anim->GetPlayLength(), false);
				}
				if (!Controller->bIsHit_Tick && !Controller->bIsHit_KnockBack)
				{
					Result = EBTNodeResult::Failed;
				}
			}
		}
	}
	return Result;
}
