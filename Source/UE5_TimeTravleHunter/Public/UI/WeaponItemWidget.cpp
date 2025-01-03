// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/WeaponItemWidget.h"
#include "UI/CurrentWeaponWidget.h"
#include "Object/PickUpItem.h"
#include "Character/Armor/WeaponBase.h"

// Engine
#include "Components/Image.h"

void UWeaponItemWidget::InitializeWeaponItem(UCurrentWeaponWidget *CurWeaponWidget, APickUpItem *WeaponItem)
{
	if (CurWeaponWidget && WeaponItem)
	{
		CurrentWeaponItem = CurWeaponWidget;
		WeaponInfo = WeaponItem;
	}
	WeaponImage->SetBrushFromTexture(WeaponItem->GetItemTexture());
	HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}

void UWeaponItemWidget::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f));
}

void UWeaponItemWidget::NativeOnMouseLeave(const FPointerEvent &InMouseEvent)
{
	HoverImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}

FReply UWeaponItemWidget::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	// 여기서 브로드캐스트랑 배열에서 인덱스 삭제 그리고 커렌트 웨폰위젯으로 데이터 전송



	CurrentWeaponItem->OnEquipWeapon.Broadcast(WeaponInfo);
	return FReply::Handled();
}
