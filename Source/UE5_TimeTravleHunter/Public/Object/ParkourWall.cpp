// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Object/ParkourWall.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Controller/PlayerCharacterController.h"

// Engine
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


AParkourWall::AParkourWall()
{
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(WallMesh);
}

void AParkourWall::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AParkourWall::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AParkourWall::OnOverlapEnd);
}

void AParkourWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParkourWall::OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APlayerCharacterController *PController = Cast<APlayerCharacterController>(Player->GetController()))
		{
			PController->bIsParkour = true;
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Parkour")), true);
		}
	}
}

void AParkourWall::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APlayerCharacterController *PController = Cast<APlayerCharacterController>(Player->GetController()))
		{
			PController->bIsParkour = false;
		}
	}
}

