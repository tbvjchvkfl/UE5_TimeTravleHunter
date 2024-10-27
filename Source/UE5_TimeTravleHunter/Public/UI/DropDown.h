// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DropDown.generated.h"

class UButton;

DECLARE_MULTICAST_DELEGATE(FOnDropItem);
DECLARE_MULTICAST_DELEGATE(FOnRemoveItem);
DECLARE_MULTICAST_DELEGATE(FOnMouseEnter);
DECLARE_MULTICAST_DELEGATE(FOnMouseLeave);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UDropDown : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnDropItem OnDropButton;
	FOnRemoveItem OnRemoveButton;
	FOnMouseEnter OnMouseEnter;
	FOnMouseLeave OnMouseLeave;

	UPROPERTY(meta = (BindWidget))
	UButton *DropButton;

	UPROPERTY(meta = (BindWidget))
	UButton *RemoveButton;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent &InMouseEvent);

	UFUNCTION()
	void DropItem();

	UFUNCTION()
	void RemoveItem();
};
