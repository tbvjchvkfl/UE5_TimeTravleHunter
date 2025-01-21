// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Object/SpawnPoint.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/ItemPoolComponent.h"
#include "Object/PickUpItem.h"

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

	}
	if (ItemSpawnType == ESpawnType::BossCharacter)
	{

	}
}

