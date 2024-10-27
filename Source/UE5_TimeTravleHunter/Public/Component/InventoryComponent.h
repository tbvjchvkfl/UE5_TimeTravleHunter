// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);

class APickUpItem;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_TIMETRAVLEHUNTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//======================================================
	//=					- Variables -					   =
	//======================================================

	FOnInventoryUpdate OnInventoryUpdate;

	FHitResult Hit;

	UPROPERTY(EditAnywhere, Category = "DropItem")
	TSubclassOf<APickUpItem> DroppingItem;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	UInventoryComponent();
	
	FORCEINLINE TMap<FVector2D, APickUpItem *> GetItemInventory() { return ItemInventory; };
	FORCEINLINE int32 GetCoinInventory() { return CoinInventory; };
	FORCEINLINE int32 GetItemInventorySize() { return InventorySize; };
	FORCEINLINE int32 GetInventoryWidth() { return InventoryWidth; };
	void CheckItem(APickUpItem *Item);
	void AddtoInventory(FVector2D ItemPosition, APickUpItem *Item, bool ModifyState);
	void RemoveFromInventory(FVector2D ItemPosition, bool ModifyState);
	void DropItem(APickUpItem* Item);
protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
	int32 InventorySize;

	UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
	int32 InventoryWidth;

	UPROPERTY(VisibleAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
	TMap<FVector2D, APickUpItem *> ItemInventory;

	UPROPERTY(VisibleAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
	TMap<FVector2D, bool> InventoryState;

	UPROPERTY()
	int32 CoinInventory;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitializeInventory();
	void AddItem(APickUpItem *Item);
	bool bIsRoomAvailable(TArray<FVector2D> Shape, FVector2D &ItemPosition) const;
};
