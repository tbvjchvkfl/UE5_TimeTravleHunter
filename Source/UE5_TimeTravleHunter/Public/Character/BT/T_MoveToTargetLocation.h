// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "T_MoveToTargetLocation.generated.h"

class UBeehavioTreeComponent;
class UPathFollowingComponent;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UT_MoveToTargetLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "TaskBase | Property")
	float AcceptableTargetRadius;


	//======================================================
	//=					- Functionary -					   =
	//======================================================
	explicit UT_MoveToTargetLocation(FObjectInitializer const &ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent *TreeComp;
};
