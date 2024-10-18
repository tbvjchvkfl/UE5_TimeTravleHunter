// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLeftMouseButtonPressed, FVector2D);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRightMouseButtonPressed, FVector2D);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseButtonEnter, FVector2D);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnLeftMouseButtonPressed OnLeftMouseButtonPressed;
	FOnRightMouseButtonPressed OnRightMouseButtonPressed;
	FOnMouseButtonEnter OnMouseButtonEnter;

	FVector2D Key;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
};
