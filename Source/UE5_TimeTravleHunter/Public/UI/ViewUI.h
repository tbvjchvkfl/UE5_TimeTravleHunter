// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ViewUI.generated.h"

class UImage;
class ADummyCharacterbyView;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UViewUI : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(meta = (BindWidget))
	UImage *ObjectViewer;

	UPROPERTY(EditAnywhere, Category = "Widget | Property")
	TSubclassOf<ADummyCharacterbyView> DummyCharacterClass;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeViewUI();

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	bool bIsDragStart;

	UPROPERTY()
	FVector2D DragStartPos;

	UPROPERTY()
	FVector2D CurrentMousePos;

	UPROPERTY()
	ADummyCharacterbyView *DummyCharacter;

	
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
};
