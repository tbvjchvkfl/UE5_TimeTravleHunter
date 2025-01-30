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
					SpawnCharacterEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyElem.Key, GetActorLocation(), GetActorRotation());
					if (SpawnCharacterEnemy)
					{
						SpawnCharacterEnemy->SetActorHiddenInGame(true);
						SpawnCharacterEnemy->SetActorEnableCollision(false);
						EnemyPool[EnemyElem.Key].AddToNPCList(SpawnCharacterEnemy);
					}
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
			if (!EnemyElem.Value.NPCListIsEmpty())
			{
				ECharacter = EnemyElem.Value.NPCList[EnemyElem.Value.NPCList.Num() - 1];
				ECharacter->SetActorHiddenInGame(false);
				ECharacter->SetActorEnableCollision(true);
				EnemyElem.Value.RemoveToNPCList();
			}
		}
	}
	return ECharacter;
}

bool AEnemyPool::CheckEnemyPoolIsEmpty(TSubclassOf<AEnemyCharacter> CharacterClass)
{
	bool ReturnValue = false;
	for (auto EnemyElem : EnemyPool)
	{
		if (CharacterClass == EnemyElem.Key)
		{
			if (EnemyElem.Value.NPCListIsEmpty())
			{
				ReturnValue = true;
			}
		}
	}
	return ReturnValue;
}

void AEnemyPool::ReturnBossEnemyPool(TSubclassOf<ABossEnemyCharacter> BossCharacterClass, ABossEnemyCharacter *Character)
{
	for (auto BossEnemyElem : BossEnemyPool)
	{
		if (BossEnemyElem.Key == BossCharacterClass)
		{
			BossEnemyElem.Value.AddToBossNPCList(Character);
		}
	}
}

ABossEnemyCharacter *AEnemyPool::UseBossEnemyPool(TSubclassOf<ABossEnemyCharacter> BossCharacterClass)
{
	ABossEnemyCharacter *BECharacter{};
	for (auto BossEnemyElem : BossEnemyPool)
	{
		if (BossEnemyElem.Key == BossCharacterClass)
		{
			if (!BossEnemyElem.Value.BossNPCListIsEmpty())
			{
				BECharacter = BossEnemyElem.Value.BossNPCList[BossEnemyElem.Value.BossNPCList.Num() - 1];
				BECharacter->SetActorHiddenInGame(false);
				BECharacter->SetActorEnableCollision(true);
				BossEnemyElem.Value.RemoveToBossNPCList();
			}
		}
	}
	return BECharacter;
}

bool AEnemyPool::CheckBossEnemyPoolIsEmpty(TSubclassOf<ABossEnemyCharacter> BossCharacterClass)
{
	bool ReturnValue = false;
	for (auto BossEnemyElem : BossEnemyPool)
	{
		if (BossCharacterClass == BossEnemyElem.Key)
		{
			if (BossEnemyElem.Value.BossNPCListIsEmpty())
			{
				ReturnValue = false;
			}
			else
			{
				ReturnValue = true;
			}
		}
	}
	return ReturnValue;
}