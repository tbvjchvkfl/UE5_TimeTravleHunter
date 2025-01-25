// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "T_StrafingMovement.generated.h"

/**
 * 
 */
UCLASS()
class UE5_TIMETRAVLEHUNTER_API UT_StrafingMovement : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================


	//======================================================
	//=					- Functionary -					   =
	//======================================================
	explicit UT_StrafingMovement(FObjectInitializer const &ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};
