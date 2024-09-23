// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ItemStruct.h"
#include "Interface/InteractionInterface.h"
#include "PickUpItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API APickUpItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	//======================================================
	//=					- Variables -					   =
	//======================================================


	//======================================================
	//=					- Functionary -					   =
	//======================================================
	APickUpItem();
	FORCEINLINE EItemType GetItemType() { return ItemType; };
	FORCEINLINE int32 GetItemAmount() { return ItemAmount; };
	
	FIntPoint GetDimensions()const
	{
		if (bIsRotated)
		{
			return FIntPoint(Dimensions.Y, Dimensions.X);
		}
		return Dimensions;
	};

	UMaterialInterface* GetItemIcon()const
	{
		if (bIsRotated)
		{
			return ItemRotationIcon;
		}
		return ItemIcon;
	};

	bool ToggleRotated() { return bIsRotated = !bIsRotated; };
protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	// Item Component
	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *ItemMesh;

	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent *CollisionSphere;

	// Item Data
	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | ItemData", meta = (AllowPrivateAccess = "true"))
	UDataTable *ItemDataTable;

	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | ItemData", meta = (AllowPrivateAccess = "true"))
	FName DesiredItemID;

	// Item Property
	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | ItemProperty")
	int32 ItemAmount;
	
	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | ItemProperty")
	UTexture2D *ItemTexture;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | ItemProperty")
	FString ItemName;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | ItemProperty")
	FString ItemDescription;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | ItemProperty")
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | Item Property", meta = (AllowPrivateAccess = "true"))
	FIntPoint Dimensions;

	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | Item Property", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface *ItemIcon;

	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | Item Property", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface *ItemRotationIcon;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Item Property")
	bool bIsRotated;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PickUpInteraction(class APlayerCharacter *Player) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
