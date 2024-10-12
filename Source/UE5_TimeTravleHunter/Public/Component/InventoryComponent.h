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

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	UInventoryComponent();
	
	FORCEINLINE TMap<FVector2D, APickUpItem *> GetItemInventory() { return ItemInventory; };
	FORCEINLINE int32 GetCoinInventory() { return CoinInventory; };

	void AddInventory(APickUpItem *Item);

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
	FIntPoint InventorySize;

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
	void AddingItem(class APickUpItem* Item, int32 TopLeftIndex);
	void AddItem(class APickUpItem *Item);
	bool bIsRoomAvailable(TArray<FVector2D> Shape) const;
	//bool bIsRoomAvailable(TArray<FVector2D>Shape, class APickUpItem *Items, int32 TopLeftIndex) const;
	bool bIsItemStackable(class APickUpItem *Item) const;
};
