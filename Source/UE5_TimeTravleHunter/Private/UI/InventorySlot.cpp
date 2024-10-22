// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlot.h"
#include "Controller/PlayerCharacterController.h"


void UInventorySlot::InitializeSlot(int32 Slot_X, int32 Slot_Y)
{
	Key = FVector2D(Slot_X, Slot_Y);
}

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
