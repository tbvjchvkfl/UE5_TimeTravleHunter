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

	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const auto ItemTable = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		if (ItemTable)
		{
			MaxQuantity = ItemTable->ItemStackData.MaxQuantity;
			ItemSize = ItemTable->ItemSize;
			ItemName = ItemTable->ItemTextData.ItemName;
			ItemDescription = ItemTable->ItemTextData.ItemDescription;
			ItemType = ItemTable->ItemType;
			ItemTexture = ItemTable->ItemAssetData.ItemIcon;
			ItemMesh->SetStaticMesh(ItemTable->ItemAssetData.ItemMesh);
		}
	}
}

void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	DesiredItemShape();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnOverlapBegin);
}

void APickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickUpItem::DesiredItemShape()
{
	ItemShape.Empty();
	for (int32 n = 0; n < ItemSize.Y; n++)
	{
		for (int32 m = 0; m < ItemSize.X; m++)
		{
			ItemShape.Add(FVector2D(m, n));
		}
	}
}

TArray<FVector2D> APickUpItem::GetShape(float Rotation) const
{
	if (Rotation != 0.0f)
	{
		TArray<FVector2D> ReturnShape;
		float SmallestX = 0.0f;
		float SmallestY = 0.0f;
		for (int32 i = 0; i < ItemShape.Num(); i++)
		{
			FVector2D RotShape = ItemShape[i].GetRotated(Rotation);
			ReturnShape.Add(RotShape);

			if (RotShape.X < SmallestX)
			{
				SmallestX = RotShape.X;
			}
			if (SmallestY > 0)
			{
				SmallestY = RotShape.Y;
			}
		}
		for (int32 i = 0; i < ReturnShape.Num(); i++)
		{
			ReturnShape[i].X -= SmallestX;
			ReturnShape[i].Y -= SmallestY;
		}
		return ReturnShape;
	}
	return ItemShape;
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

