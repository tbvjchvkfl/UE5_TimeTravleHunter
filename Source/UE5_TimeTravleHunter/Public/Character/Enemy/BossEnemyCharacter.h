// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossEnemyCharacter.generated.h"

UCLASS()
class UE5_TIMETRAVLEHUNTER_API ABossEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//======================================================
	//=					- Variables -					   =
	//======================================================



	//======================================================
	//=					- Functionary -					   =
	//======================================================
	ABossEnemyCharacter();

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	


	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
