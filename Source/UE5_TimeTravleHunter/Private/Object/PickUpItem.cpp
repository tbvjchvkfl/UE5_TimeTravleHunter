// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Object/PickUpItem.h"
#include "Character/Armor/WeaponBase.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/InventoryComponent.h"

// Engine
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DataTable.h"

APickUpItem::APickUpItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(ItemMesh);
	CollisionSphere->InitSphereRadius(200.0f);
	
	CurrentQuantity = 1;
}

void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	InitializeItemData();
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
			if (SmallestY > RotShape.Y)
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

FIntPoint APickUpItem::GetMaxSize(float Rotation, bool DefaultOverride)
{
	FIntPoint ReturnValue = FIntPoint(0, 0);
	float Max_X = 0.0f;
	float Max_Y = 0.0f;

	if (DefaultOverride)
	{
		for (int32 i = 0; i < ItemShape.Num(); i++)
		{
			if (Max_X < ItemShape[i].X)
			{
				Max_X = ItemShape[i].X;
			}
			if (Max_Y < ItemShape[i].Y)
			{
				Max_Y = ItemShape[i].Y;
			}
		}
	}
	else
	{
		for (int32 i = 0; i < GetShape(Rotation).Num(); i++)
		{
			if (Max_X < GetShape(Rotation)[i].X)
			{
				Max_X = GetShape(Rotation)[i].X;
			}
			if (Max_Y < GetShape(Rotation)[i].Y)
			{
				Max_Y = GetShape(Rotation)[i].Y;
			}
		}
	}
	ReturnValue.X = static_cast<int32>(Max_X + 1);
	ReturnValue.Y = static_cast<int32>(Max_Y + 1);
	return ReturnValue;
}

void APickUpItem::InitializeItemData()
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const auto ItemTable = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		if (ItemTable)
		{
			ItemNumber = ItemTable->ItemNumber;
			ItemSize = ItemTable->ItemSize;
			MaxQuantity = ItemTable->ItemStackData.MaxQuantity;
			ItemType = ItemTable->ItemType;
			WeaponType = ItemTable->WeaponType;
			ItemName = ItemTable->ItemTextData.ItemName;
			ItemDescription = ItemTable->ItemTextData.ItemDescription;
			ItemTexture = ItemTable->ItemAssetData.ItemIcon;
			ItemMesh->SetStaticMesh(ItemTable->ItemAssetData.ItemMesh);
		}
	}
}

void APickUpItem::PickUpInteraction(APlayerCharacter *Player)
{
	if (!IsPendingKillPending())
	{
		if (auto ItemInventory = Player->GetItemInventory())
		{
			ItemInventory->CheckItem(this);
			// Destroy가 아니라 Hidden으로 바꿀 것.
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			//Destroy();
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