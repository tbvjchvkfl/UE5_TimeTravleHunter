// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

class APickUpItem;
class APlayerCharacter;
class AEnemyCharacter;
class AEnemyCharacterController;
class UItemPoolComponent;
class AEnemyPool;

UENUM()
enum class ESpawnType : uint8
{
	PickUpItem UMETA(DisplayName = "Item"),
	EnemyCharacter UMETA(DisplayName = "Npc"),
	BossCharacter UMETA(DisplayName = "Boss")
};

UCLASS()
class UE5_TIMETRAVLEHUNTER_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Basic Info | SpawnType")
	ESpawnType ItemSpawnType;

	UPROPERTY(EditAnywhere, Category = "Basic Info | Item Spawn Info")
	FName ItemRowName;

	UPROPERTY(EditAnywhere, Category = "Basic Info | Item Spawn Info")
	int32 ItemQuantity;

	UPROPERTY(EditAnywhere, Category = "Basic Info | Item Spawn Info")
	TSubclassOf<AEnemyCharacter> SpawnCharacterClass;

	UPROPERTY(EditAnywhere, Category = "Basic Info | Item Spawn Info")
	TSubclassOf<AEnemyPool> EnemyPoolClass;

	UPROPERTY(EditAnywhere, Category = "Basic Info | Item Spawn Info")
	TSubclassOf<AEnemyCharacterController> EnemyControllerClass;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	ASpawnPoint();

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	APlayerCharacter *Player;
	
	UPROPERTY()
	APickUpItem *SpawnItem;

	UPROPERTY()
	UItemPoolComponent *ItemPoolComponent;

	UPROPERTY()
	TArray<APickUpItem *>ItemPoolList;

	UPROPERTY()
	AEnemyCharacter *SpawnCharacter;

	UPROPERTY()
	AEnemyCharacterController *SpawnCharacterController;

	UPROPERTY()
	AEnemyPool *EnemyPool;


	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void SpawnObject();
	void PossessNPCController(APawn *InPawn);
};
