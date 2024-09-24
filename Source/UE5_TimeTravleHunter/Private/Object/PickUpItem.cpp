// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Object/PickUpItem.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/InventoryComponent.h"


// Engine
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

APickUpItem::APickUpItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(ItemMesh);
	CollisionSphere->InitSphereRadius(200.0f);

	Dimensions = FIntPoint(1, 1);

	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const auto ItemTable = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		if (ItemTable)
		{
			ItemType = ItemTable->ItemType;
			ItemAmount = ItemTable->Amount;
			ItemMesh->SetStaticMesh(ItemTable->ItemAssetData.ItemMesh);
		}
	}
}

void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnOverlapBegin);
}

void APickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpItem::PickUpInteraction(APlayerCharacter *Player)
{
	if (!IsPendingKillPending())
	{
		if (auto ItemInventory = Player->GetItemInventory())
		{
			ItemInventory->AddInventory(this);
			Destroy();
		}
	}
}

void APickUpItem::OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Press E Button")), true);
	}
}

