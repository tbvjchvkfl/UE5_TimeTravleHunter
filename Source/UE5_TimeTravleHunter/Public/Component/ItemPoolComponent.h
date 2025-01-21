// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemStruct.h"
#include "ItemPoolComponent.generated.h"

class APickUpItem;

USTRUCT()
struct FItemPoolStruct
{
	GENERATED_USTRUCT_BODY()

	TArray<APickUpItem *> ItemList;
	FItemPoolStruct()
	{
		ItemList.Empty();
	}
	void AddToItemList(APickUpItem* Item)
	{
		ItemList.Add(Item);
	}
	void RemoveToItemList()
	{
		ItemList.Pop();
	}
	bool ItemListIsEmpty()
	{
		return ItemList.IsEmpty();
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_TIMETRAVLEHUNTER_API UItemPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Class | BasicProperty")
	TSubclassOf<APickUpItem> PickUpItemClass;

	UPROPERTY()
	bool bIsFillItemPool;

	UPROPERTY(EditAnywhere, Category = "Class | BasicProperty")
	TMap<FName, int32> EditList;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	UItemPoolComponent();
	void ReturnItemToPool(APickUpItem *Item);
	APickUpItem *UseItemOfPool(FName ItemRowName);
	bool CheckItemPoolIsEmpty();

	FORCEINLINE TMap<FName, FItemPoolStruct> GetItemPool()const { return ItemPool; };

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = "true"))
	UDataTable *ItemDataTable;

	UPROPERTY(VisibleAnywhere, Category = "PoolMap")
	TMap<FName, FItemPoolStruct> ItemPool;

	UPROPERTY()
	APickUpItem *PickUpItem;


	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void InitializePool();
	
};
