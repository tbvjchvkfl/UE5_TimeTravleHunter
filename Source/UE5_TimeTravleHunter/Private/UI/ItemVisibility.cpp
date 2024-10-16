// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/ItemVisibility.h"


// Engine
#include "Components/Image.h"
#include "Components/SizeBox.h"

void UItemVisibility::ChangeColor(FLinearColor Color)
{
    /*if (!IsAnimationPlaying())
    {
        HoverImage->SetColorAndOpacity(Color);
    }*/
    HoverImage->SetColorAndOpacity(Color);
}

void UItemVisibility::VisibilityWidgetInit(float OverrideSize)
{
    WidgetSizeBox->SetWidthOverride(OverrideSize);
    WidgetSizeBox->SetHeightOverride(OverrideSize);
}

FReply UItemVisibility::NativeOnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
    Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);

    OnMouseButtonPressed.Broadcast(MyGeometry, MouseEvent);

    return FReply::Handled();
}
