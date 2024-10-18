// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemVisibility.generated.h"

class USizeBox;
class UImage;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMouseButtonDown, const FGeometry &, const FPointerEvent &);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UItemVisibility : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnMouseButtonDown OnMouseButtonPressed;

	UPROPERTY(meta = (BindWidget))
	USizeBox *WidgetSizeBox;

	UPROPERTY(meta = (BindWidget))
	UImage *HoverImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation *Blink;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void ChangeColor(FLinearColor Color);
	void VisibilityWidgetInit(float OverrideSize);
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	void StartBlink();
	void StopBlink();
};
