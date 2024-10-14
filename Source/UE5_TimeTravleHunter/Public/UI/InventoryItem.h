// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.generated.h"

class APickUpItem;
class USizeBox;
class UImage;
class UTextBlock;

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemovedItem, APickUpItem *);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UInventoryItem : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	//FOnRemovedItem OnRemoveItem;

	UPROPERTY(meta = (BindWidget))
	USizeBox *BackGroundSizeBox;

	UPROPERTY(meta = (BindWidget))
	UImage *ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *ItemQuantityText;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeInventoryItem(APickUpItem* PickUpItem);

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "ItemWidget | Property", meta = (AllowPrivateAccess = "true"))
	float ItemTileSize;

	UPROPERTY()
	FVector2D Size;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
};
