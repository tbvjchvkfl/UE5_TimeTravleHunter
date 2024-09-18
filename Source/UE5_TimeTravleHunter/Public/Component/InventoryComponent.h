// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);

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
	
	FORCEINLINE TArray<class APickUpItem*> GetItemInventory() { return ItemInventory; };
	FORCEINLINE int32 GetCoinInventory() { return CoinInventory; };
	FORCEINLINE int32 GetColumns() { return Columns; };
	FORCEINLINE int32 GetRows() { return Rows; };

	void AddInventory(class APickUpItem *Item);
protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	TArray<class APickUpItem*> ItemInventory;

	UPROPERTY()
	int32 CoinInventory;

	UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
	int32 Rows;

	UPROPERTY(VisibleAnywhere, Category = "Inventory | Property")
	bool bIsDirty;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddingItem(class APickUpItem* Item, int32 TopLeftIndex);
	void AddItem(class APickUpItem *Item);
	bool bIsRoomAvailable(class APickUpItem *Items, int32 TopLeftIndex) const;
};
