// Fill out your copyright notice in the Description page of Project Settings.

// GameFrameWork
#include "UI/DropDown.h"

// Engine
#include "Components/Button.h"

void UDropDown::NativeConstruct()
{
	//DropButton->OnClicked.AddDynamic(this, &UDropDown::DropItem);
	//RemoveButton->OnClicked.AddDynamic(this, &UDropDown::RemoveItem);
}

void UDropDown::DropItem()
{
	OnDropButton.Broadcast();
}

void UDropDown::RemoveItem()
{
	OnRemoveButton.Broadcast();
}

void UDropDown::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	OnMouseEnter.Broadcast();
}

void UDropDown::NativeOnMouseLeave(const FPointerEvent &InMouseEvent)
{
	OnMouseLeave.Broadcast();
}
