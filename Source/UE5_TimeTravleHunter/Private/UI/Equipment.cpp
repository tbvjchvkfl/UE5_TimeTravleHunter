// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/Equipment.h"
#include "UI/EquipmentSlot.h"
#include "UI/EquipmentContents.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/InventoryComponent.h"

// Engine
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"

void UEquipment::InitializeEquipment()
{
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	MainWeaponButton->OnClicked.AddDynamic(this, &UEquipment::OnMainWeaponClicked);
	SubWeaponButton->OnClicked.AddDynamic(this, &UEquipment::OnSubWeaponClicked);
	RangedWeaponButton->OnClicked.AddDynamic(this, &UEquipment::OnRangedWeaponClicked);
}

void UEquipment::OnMainWeaponClicked()
{
	if (EquipmentContentsWidget)
	{
		EquipmentContents = CreateWidget<UEquipmentContents>(GetOwningPlayer(), EquipmentContentsWidget);
		auto SlotPanel = MainWeaponCanvas->AddChildToCanvas(EquipmentContents);
		if (auto ButtonSlot = Cast<UCanvasPanelSlot>(MainWeaponBorder->Slot))
		{
			// 위치 수정
			SlotPanel->SetPosition(ButtonSlot->GetPosition());
		}
	}
}

void UEquipment::OnSubWeaponClicked()
{
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString("SubWeaponClick"));
}

void UEquipment::OnRangedWeaponClicked()
{
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString("RangedWeaponClick"));
}
