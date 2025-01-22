// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyCharacterController.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class AEnemyCharacter;
class UEnemyAnimInstance;
class UBlackBoardComponent;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API AEnemyCharacterController : public AAIController
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Controller | Essential Data")
	UBehaviorTree *EnemyTree;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	FORCEINLINE AEnemyCharacter *GetOwningCharacter() const { return OwningCharacter; };

	AEnemyCharacterController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn *InPawn) override;
	virtual void OnUnPossess() override;

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	AEnemyCharacter *OwningCharacter;
	
	UPROPERTY()
	UEnemyAnimInstance *OwningInstance;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void SetUpBehaviorAndBoard();
};
