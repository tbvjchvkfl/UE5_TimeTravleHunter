// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);

class APickUpItem;
class AWeaponBase;
class APlayerCharacter;
class UItemPoolComponent;

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
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	UInventoryComponent();
	
	FORCEINLINE int32 GetCoinInventory() { return CoinInventory; };

	FORCEINLINE TMap<FVector2D, APickUpItem *> GetItemInventory() { return ItemInventory; };
	FORCEINLINE int32 GetItemInventorySize() { return InventorySize; };
	FORCEINLINE int32 GetInventoryWidth() { return InventoryWidth; };

	FORCEINLINE TArray<bool> GetRoomCheckingInventory() { return RoomCheckingInventory; };
	FORCEINLINE TArray<APickUpItem *> GetWeaponInventory() { return EquipInventory; };
	FORCEINLINE int32 GetWeaponInventorySize() { return EquipInventorySize; };

	void CheckItem(APickUpItem *Item);
	void AddtoInventory(FVector2D ItemPosition, APickUpItem *Item, bool ModifyState);
	void RemoveFromInventory(FVector2D ItemPosition, bool ModifyState);
	void DropItem(APickUpItem* Item);

	void AddWeaponInventory(APickUpItem *Item);
	void RemoveWeaponInventory(int32 InventoryIndex);
protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(VisibleAnywhere, Category = "Property | Pooling")
	APlayerCharacter *Player;

	UPROPERTY(VisibleAnywhere, Category = "Property | Pooling")
	UItemPoolComponent *ItemPoolComponent;

	UPROPERTY(EditAnywhere, Category = "Inventory | Consumable")
	int32 InventorySize;

	UPROPERTY(EditAnywhere, Category = "Inventory | Consumable")
	int32 InventoryWidth;

	UPROPERTY(VisibleAnywhere, Category = "Inventory | Consumable")
	TMap<FVector2D, APickUpItem *> ItemInventory;

	UPROPERTY(VisibleAnywhere, Category = "Inventory | Consumable")
	TMap<FVector2D, bool> InventoryState;

	UPROPERTY(VisibleAnywhere, Category = "Inventory | Coin")
	int32 CoinInventory;

	UPROPERTY(VisibleAnywhere, Category = "Inventory | Weapon")
	TArray<APickUpItem *> EquipInventory;

	UPROPERTY(VisibleAnywhere, Category = "Inventory | Weapon")
	TArray<bool> RoomCheckingInventory;

	UPROPERTY(EditAnywhere, Category = "Inventory | Weapon")
	int32 EquipInventorySize;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitializeInventory();
	void InitilaizeEquipInventory();
	void SetPropertyData();
	void AddItem(APickUpItem *Item);
	void AddWeapon(APickUpItem *Item);
	bool bIsRoomAvailable(TArray<FVector2D> Shape, FVector2D &ItemPosition) const;
};
