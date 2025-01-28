// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Character/Enemy/EnemyPool.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/Enemy/BossEnemyCharacter.h"


// Engine


AEnemyPool::AEnemyPool()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyPool::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
}

void AEnemyPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyPool::InitializePool()
{
	InitEnemyPool();
	InitBossEnemyPool();
}

void AEnemyPool::InitEnemyPool()
{
	if (!EditEnemyPoolList.IsEmpty())
	{
		for (auto EnemyElem : EditEnemyPoolList)
		{
			if (EnemyElem.Key)
			{
				EnemyPool.Add(EnemyElem.Key);
				for (int32 i = 0; i < EnemyElem.Value; i++)
				{
					AEnemyCharacter *SpawnCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyElem.Key);
					SpawnCharacter->SetActorHiddenInGame(true);
					SpawnCharacter->SetActorEnableCollision(false);
					EnemyPool[EnemyElem.Key].AddToNPCList(SpawnCharacter);
				}
			}
		}
	}
}

void AEnemyPool::InitBossEnemyPool()
{
	if (!EditBossEnemyPoolList.IsEmpty())
	{
		for (auto BossEnemyElem : EditBossEnemyPoolList)
		{
			if (BossEnemyElem.Key)
			{
				BossEnemyPool.Add(BossEnemyElem.Key);
				for (int32 i = 0; i < BossEnemyElem.Value; i++)
				{
					ABossEnemyCharacter *SpawnCharacter = GetWorld()->SpawnActor<ABossEnemyCharacter>(BossEnemyElem.Key);
					SpawnCharacter->SetActorHiddenInGame(true);
					SpawnCharacter->SetActorEnableCollision(false);
					BossEnemyPool[BossEnemyElem.Key].AddToBossNPCList(SpawnCharacter);
				}
			}
		}
	}
}

void AEnemyPool::ReturnEnemyPool(TSubclassOf<AEnemyCharacter> CharacterClass, AEnemyCharacter *Character)
{
	for (auto EnemyElem : EnemyPool)
	{
		if (EnemyElem.Key == CharacterClass)
		{
			EnemyElem.Value.AddToNPCList(Character);
		}
	}
}

AEnemyCharacter *AEnemyPool::UseEnemyPool(TSubclassOf<AEnemyCharacter> CharacterClass)
{
	AEnemyCharacter *ECharacter{};
	for (auto EnemyElem : EnemyPool)
	{
		if (EnemyElem.Key == CharacterClass)
		{
			ECharacter = EnemyElem.Value.NPCList[EnemyElem.Value.NPCList.Num() - 1];
			ECharacter->SetActorHiddenInGame(false);
			ECharacter->SetActorEnableCollision(true);
			EnemyElem.Value.RemoveToNPCList();
		}
	}
	return ECharacter;
}

bool AEnemyPool::CheckEnemyPoolIsEmpty()
{
	return false;
}

void AEnemyPool::ReturnBossEnemyPool(TSubclassOf<ABossEnemyCharacter> BossCharacterClass, ABossEnemyCharacter *Character)
{
	for (auto EnemyElem : BossEnemyPool)
	{
		if (EnemyElem.Key == BossCharacterClass)
		{
			EnemyElem.Value.AddToBossNPCList(Character);
		}
	}
}

ABossEnemyCharacter *AEnemyPool::UseBossEnemyPool(TSubclassOf<ABossEnemyCharacter> BossCharacterClass)
{
	ABossEnemyCharacter *BECharacter{};
	for (auto EnemyElem : BossEnemyPool)
	{
		if (EnemyElem.Key == BossCharacterClass)
		{
			BECharacter = EnemyElem.Value.BossNPCList[EnemyElem.Value.BossNPCList.Num() - 1];
			BECharacter->SetActorHiddenInGame(false);
			BECharacter->SetActorEnableCollision(true);
			EnemyElem.Value.RemoveToBossNPCList();
		}
	}
	return BECharacter;
}

bool AEnemyPool::CheckBossEnemyPoolIsEmpty()
{
	return false;
}