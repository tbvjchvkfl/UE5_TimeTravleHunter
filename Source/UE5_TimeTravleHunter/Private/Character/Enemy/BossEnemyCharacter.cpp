// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Character/Enemy/BossEnemyCharacter.h"

// Engine



ABossEnemyCharacter::ABossEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

