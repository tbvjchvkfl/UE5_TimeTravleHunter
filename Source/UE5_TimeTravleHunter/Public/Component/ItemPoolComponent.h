// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemStruct.h"
#include "ItemPoolComponent.generated.h"

class APickUpItem;

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
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	UItemPoolComponent();
	FORCEINLINE TArray<APickUpItem *> GetItemPool()const { return ItemPool; };
	FORCEINLINE APickUpItem *GetItemAccessToIndex(int32 Index)const { return ItemPool[Index]; };
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = "true"))
	UDataTable *ItemDataTable;

	UPROPERTY(VisibleAnywhere, Category = "Pool Data", meta = (AllowPrivateAccess = "true"))
	TArray<APickUpItem *> ItemPool;

	UPROPERTY()
	APickUpItem *PickUpItem;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializedItemData();
};
