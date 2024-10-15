// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemVisibility.generated.h"

class USizeBox;
class UImage;


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMouseButtonDown, FGeometry, FPointerEvent);


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

	UPROPERTY(EditAnywhere, Category = "WidgetProperty")
	float WidthOverride;

	UPROPERTY(EditAnywhere, Category = "WidgetProperty")
	float HeightOverride;

	//======================================================
	//=					- Functionary -					   =
	//======================================================

	void ChangeColor(FLinearColor Color);

	virtual void NativeConstruct()override;
	//virtual FReply OnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent);
};
