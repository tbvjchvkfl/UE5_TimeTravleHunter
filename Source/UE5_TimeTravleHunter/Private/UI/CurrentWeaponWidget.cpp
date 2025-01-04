// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/CurrentWeaponWidget.h"
#include "UI/WeaponItemWidget.h"
#include "UI/EquipWeaponWidget.h"
#include "Object/PickUpItem.h"
#include "Character/Player/PlayerCharacter.h"

// Engine
#include "Components/SizeBox.h"
#include "Components/Image.h"

void UCurrentWeaponWidget::InitializeCurrentWeaponImage()
{
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	ImageSizeBox->SetWidthOverride(200.0f);
	ImageSizeBox->SetHeightOverride(200.0f);
	EquipWeaponImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	OnEquipWeapon.AddUObject(this, &UCurrentWeaponWidget::EquipWeapon);
}

void UCurrentWeaponWidget::AddWeapon()
{

}

void UCurrentWeaponWidget::EquipWeapon(APickUpItem *WeaponItem)
{
	if (WeaponItem)
	{
		GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString("MouseClick"));
		EquipWeaponImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		EquipWeaponImage->SetBrushFromTexture(WeaponItem->GetItemTexture());
		CurEuipItem = WeaponItem;
		// 여기서 Player가 가지고 있는 Weapon에 정보 전달!!!
	}
}

FReply UCurrentWeaponWidget::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// StartAnimation
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString("AnimationStart"));
	}
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
