// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "T_MoveToRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class UE5_TIMETRAVLEHUNTER_API UT_MoveToRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "TaskBase | Property")
	float SearchRadius;


	//======================================================
	//=					- Functionary -					   =
	//======================================================
	explicit UT_MoveToRandomLocation(FObjectInitializer const &ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};
