// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/ViewUI.h"
#include "Object/DummyCharacterbyView.h"

// Engine
#include "Kismet/GameplayStatics.h"


void UViewUI::InitializeViewUI()
{
	bIsDragStart = false;
	if (DummyCharacterClass)
	{
		TArray<AActor *>ActorArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), DummyCharacterClass, ActorArray);
		for (auto ArrayElem : ActorArray)
		{
			if (ArrayElem)
			{
				DummyCharacter = Cast<ADummyCharacterbyView>(ArrayElem);
				break;
			}
		}
	}
}

FReply UViewUI::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FReply Result = FReply::Unhandled();

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragStart = true;
		DragStartPos = InMouseEvent.GetScreenSpacePosition();
		Result = FReply::Handled();
	}
	
	return Result;
}

FReply UViewUI::NativeOnMouseButtonUp(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	bIsDragStart = false;

	return FReply::Handled();
}

FReply UViewUI::NativeOnMouseMove(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	FReply Result = FReply::Unhandled();

	if (bIsDragStart)
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			CurrentMousePos = InMouseEvent.GetScreenSpacePosition();
			FVector2D MouseDragStateDelta = DragStartPos - CurrentMousePos;
			if (CurrentMousePos.X < DragStartPos.X)
			{
				DummyCharacter->TurnLeft();
				DragStartPos.X = CurrentMousePos.X;
			}
			if (CurrentMousePos.X > DragStartPos.X)
			{
				DummyCharacter->TurnRight();
				DragStartPos.X = CurrentMousePos.X;
			}
			/*if (CurrentMousePos.Y < DragStartPos.Y)
			{
				DummyCharacter->TurnUp();
				DragStartPos.Y = CurrentMousePos.Y;
			}
			if (CurrentMousePos.Y > DragStartPos.Y)
			{
				DummyCharacter->TurnDown();
				DragStartPos.Y = CurrentMousePos.Y;
			}*/
			Result = FReply::Handled();
		}
	}
	
	return Result;
}