// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlot.h"

void UInventorySlot::NativeOnMouseEnter(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
	OnMouseButtonEnter.Broadcast(Key);
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
	Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnLeftMouseButtonPressed.Broadcast(Key);
		return FReply::Handled();
	}
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnRightMouseButtonPressed.Broadcast(Key);
		return FReply::Handled();
	}
	return FReply::Handled();
}
