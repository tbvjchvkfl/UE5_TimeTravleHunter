// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/ItemVisibility.h"


// Engine
#include "Components/Image.h"
#include "Components/SizeBox.h"


void UItemVisibility::VisibilityWidgetInit(float OverrideSize)
{
    WidgetSizeBox->SetWidthOverride(OverrideSize);
    WidgetSizeBox->SetHeightOverride(OverrideSize);
}

void UItemVisibility::ChangeColor(FLinearColor Color)
{
    if (!IsAnimationPlaying(Blink))
    {
        HoverImage->SetColorAndOpacity(Color);
    }
}

FReply UItemVisibility::NativeOnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
    Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);

    OnMouseButtonPressed.Broadcast(MyGeometry, MouseEvent);

    return FReply::Handled();
}

void UItemVisibility::StartBlink()
{
    PlayAnimation(Blink, 0.0f, 0);
}

void UItemVisibility::StopBlink()
{
    StopAnimation(Blink);
}
