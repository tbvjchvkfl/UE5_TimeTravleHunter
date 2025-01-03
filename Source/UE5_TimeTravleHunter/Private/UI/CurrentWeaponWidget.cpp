// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/CurrentWeaponWidget.h"
#include "UI/WeaponItemWidget.h"
#include "UI/EquipWeaponWidget.h"
#include "Object/PickUpItem.h"

// Engine
#include "Components/SizeBox.h"
#include "Components/Image.h"

void UCurrentWeaponWidget::InitializeCurrentWeaponImage()
{
	ImageSizeBox->SetWidthOverride(200.0f);
	ImageSizeBox->SetHeightOverride(200.0f);
	EquipWeaponImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	OnEquipWeapon.AddUObject(this, &UCurrentWeaponWidget::EquipWeapon);
}

void UCurrentWeaponWidget::EquipWeapon(APickUpItem *WeaponItem)
{
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString("MouseClick"));
}

FReply UCurrentWeaponWidget::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	// StartAnimation
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString("AnimationStart"));
	return FReply::Handled();
}

void UCurrentWeaponWidget::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f));
}

void UCurrentWeaponWidget::NativeOnMouseLeave(const FPointerEvent &InMouseEvent)
{
	HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}
