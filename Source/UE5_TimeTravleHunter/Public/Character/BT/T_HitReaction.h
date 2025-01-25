// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "T_HitReaction.generated.h"

/**
 * 
 */
UCLASS()
class UE5_TIMETRAVLEHUNTER_API UT_HitReaction : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FTimerHandle DesiredNodeResultTimer;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	explicit UT_HitReaction(FObjectInitializer const &ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};
