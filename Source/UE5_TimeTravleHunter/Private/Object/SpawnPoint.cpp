// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Object/SpawnPoint.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/ItemPoolComponent.h"
#include "Object/PickUpItem.h"
#include "Character/Enemy/EnemyPool.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/Controller/EnemyCharacterController.h"


// Engine
#include "Kismet/GameplayStatics.h"

ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemSpawnType = ESpawnType::PickUpItem;
	ItemQuantity = 0;
}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ItemPoolComponent = Player->GetItemPoolComponent();
	if (ItemPoolComponent)
	{
		FTimerHandle SpawnTimer;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer,  [this]()
			{
				if (ItemPoolComponent->bIsFillItemPool)
				{
					SpawnObject();
				}
			}, 0.2f, false
		);
	}
}

void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnPoint::SpawnObject()
{
	if (ItemSpawnType == ESpawnType::PickUpItem)
	{
		SpawnItem = ItemPoolComponent->UseItemOfPool(ItemRowName);
		if (SpawnItem)
		{
			SpawnItem->SetActorLocation(GetActorLocation());
			SpawnItem->SetCurrentQuantity(ItemQuantity);
			this->Destroy();
		}
	}
	if (ItemSpawnType == ESpawnType::EnemyCharacter)
	{
		if (EnemyPoolClass)
		{
			TArray<AActor *>ActorArray;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyPoolClass, ActorArray);
			for (auto ActorElem : ActorArray)
			{
				if (ActorElem)
				{
					EnemyPool = Cast<AEnemyPool>(ActorElem);
					break;
				}
			}
			if (EnemyPool)
			{
				if (SpawnCharacterClass && !EnemyPool->CheckEnemyPoolIsEmpty(SpawnCharacterClass))
				{
					SpawnCharacter = EnemyPool->UseEnemyPool(SpawnCharacterClass);
					SpawnCharacter->SetActorLocation(GetActorLocation());
					this->Destroy();
				}
			}
		}
	}
	if (ItemSpawnType == ESpawnType::BossCharacter)
	{

	}
}

void ASpawnPoint::PossessNPCController(APawn *InPawn)
{
	if (EnemyControllerClass)
	{
		SpawnCharacterController = GetWorld()->SpawnActor<AEnemyCharacterController>(EnemyControllerClass);
		SpawnCharacterController->OnPossess(InPawn);
	}
}

