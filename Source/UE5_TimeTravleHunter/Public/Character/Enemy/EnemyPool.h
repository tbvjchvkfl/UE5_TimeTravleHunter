// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPool.generated.h"

class AEnemyCharacter;
class ABossEnemyCharacter;

USTRUCT()
struct FEnemyPoolStruct
{
	GENERATED_USTRUCT_BODY()

	TArray<AEnemyCharacter *> NPCList;
	FEnemyPoolStruct()
	{
		NPCList.Empty();
	}
	void AddToNPCList(AEnemyCharacter *Character)
	{
		NPCList.Add(Character);
	}
	void RemoveToNPCList()
	{
		NPCList.Pop();
	}
	bool NPCListIsEmpty()
	{
		return NPCList.IsEmpty();
	}
};

USTRUCT()
struct FBossEnemyPoolStruct
{
	GENERATED_USTRUCT_BODY()

	TArray<ABossEnemyCharacter *> BossNPCList;
	FBossEnemyPoolStruct()
	{
		BossNPCList.Empty();
	}
	void AddToBossNPCList(ABossEnemyCharacter *Character)
	{
		BossNPCList.Add(Character);
	}
	void RemoveToBossNPCList()
	{
		BossNPCList.Pop();
	}
	bool BossNPCListIsEmpty()
	{
		return BossNPCList.IsEmpty();
	}
};

UCLASS()
class UE5_TIMETRAVLEHUNTER_API AEnemyPool : public AActor
{
	GENERATED_BODY()
	
public:	
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "PoolDefault | Spawn Info")
	TSubclassOf<AEnemyCharacter> SpawnCharacterEnemyClass;

	UPROPERTY(EditAnywhere, Category = "PoolDefault | Spawn Info")
	TMap<TSubclassOf<AEnemyCharacter>, int32> EditEnemyPoolList;

	UPROPERTY(EditAnywhere, Category = "PoolDefault | Spawn Info")
	TMap<TSubclassOf<ABossEnemyCharacter>, int32> EditBossEnemyPoolList;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	AEnemyPool();

	void ReturnEnemyPool(TSubclassOf<AEnemyCharacter> CharacterClass, AEnemyCharacter* Character);
	AEnemyCharacter *UseEnemyPool(TSubclassOf<AEnemyCharacter> CharacterClass);
	bool CheckEnemyPoolIsEmpty(TSubclassOf<AEnemyCharacter> CharacterClass);

	void ReturnBossEnemyPool(TSubclassOf<ABossEnemyCharacter> BossCharacterClass, ABossEnemyCharacter *Character);
	ABossEnemyCharacter *UseBossEnemyPool(TSubclassOf<ABossEnemyCharacter> BossCharacterClass);
	bool CheckBossEnemyPoolIsEmpty(TSubclassOf<ABossEnemyCharacter> BossCharacterClass);

	FORCEINLINE TMap<TSubclassOf<AEnemyCharacter>, FEnemyPoolStruct> GetEnemyPool()const { return EnemyPool; };
	FORCEINLINE TMap<TSubclassOf<ABossEnemyCharacter>, FBossEnemyPoolStruct> GetBossEnemyPool() const { return BossEnemyPool; };

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	AEnemyCharacter *SpawnCharacterEnemy;

	UPROPERTY()
	TMap<TSubclassOf<AEnemyCharacter>, FEnemyPoolStruct> EnemyPool;

	UPROPERTY()
	TMap<TSubclassOf<ABossEnemyCharacter>, FBossEnemyPoolStruct> BossEnemyPool;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void InitializePool();
	void InitEnemyPool();
	void InitBossEnemyPool();
};
