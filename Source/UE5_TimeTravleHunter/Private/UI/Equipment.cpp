// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "UI/Equipment.h"
#include "UI/EquipmentContents.h"
#include "UI/EquipmentSlot.h"
#include "UI/ViewUI.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/InventoryComponent.h"
#include "Component/WeaponComponent.h"
#include "Object/PickUpItem.h"

// Engine
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Styling/SlateBrush.h"

void UEquipment::InitializeEquipment()
{
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		WeaponComponent = Player->GetWeaponComponent();
	}
	ViewUI->InitializeViewUI();

	MainWeaponButton->OnClicked.AddDynamic(this, &UEquipment::OnMainWeaponClicked);
	SubWeaponButton->OnClicked.AddDynamic(this, &UEquipment::OnSubWeaponClicked);
	RangedWeaponButton->OnClicked.AddDynamic(this, &UEquipment::OnRangedWeaponClicked);
	OnEquipWeapon.AddUObject(this, &UEquipment::EquipWeaponItem);
}

void UEquipment::OnMainWeaponClicked()
{
	bIsActiveMainWeapon_BTN = true;
	bIsActiveSubWeapon_BTN = false;
	bIsActiveRangedWeapon_BTN = false;
	SubWeaponCanvas->RemoveChild(EquipmentContents);
	RangedWeaponCanvas->RemoveChild(EquipmentContents);
	ButtonInteraction(MainWeaponCanvas, MainWeaponBorder);
}

void UEquipment::OnSubWeaponClicked()
{
	bIsActiveMainWeapon_BTN = false;
	bIsActiveSubWeapon_BTN = true;
	bIsActiveRangedWeapon_BTN = false;
	MainWeaponCanvas->RemoveChild(EquipmentContents);
	RangedWeaponCanvas->RemoveChild(EquipmentContents);
	ButtonInteraction(SubWeaponCanvas, SubWeaponBorder);
}

void UEquipment::OnRangedWeaponClicked()
{
	bIsActiveMainWeapon_BTN = false;
	bIsActiveSubWeapon_BTN = false;
	bIsActiveRangedWeapon_BTN = true;
	MainWeaponCanvas->RemoveChild(EquipmentContents);
	SubWeaponCanvas->RemoveChild(EquipmentContents);
	ButtonInteraction(RangedWeaponCanvas, RangedWeaponBorder);
}

FReply UEquipment::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	RemoveItemListWIdget(MainWeaponCanvas);
	RemoveItemListWIdget(SubWeaponCanvas);
	RemoveItemListWIdget(RangedWeaponCanvas);
	bIsActiveMainWeapon_BTN = false;
	bIsActiveSubWeapon_BTN = false;
	bIsActiveRangedWeapon_BTN = false;
	return FReply::Handled();
}

void UEquipment::ButtonInteraction(UCanvasPanel *InCanvas, UBorder *InBorder)
{
	if (CanCheckCreateWidget(InCanvas))
	{
		if (EquipmentContentsWidget)
		{
			EquipmentContents = CreateWidget<UEquipmentContents>(GetOwningPlayer(), EquipmentContentsWidget);
			if (EquipmentContents)
			{
				EquipmentContents->InitializeEquipmentContents(this);
				EquipmentContents->OnAddWeaponWidget.AddUObject(this, &UEquipment::AddWeaponItem);
				EquipmentContents->OnRemoveWeaponWidget.AddUObject(this, &UEquipment::RemoveWeaponItem);

				if (auto SlotPanel = InCanvas->AddChildToCanvas(EquipmentContents))
				{
					if (auto ButtonSlot = Cast<UCanvasPanelSlot>(InBorder->Slot))
					{
						SlotPanel->SetSize(FVector2D(ItemListSize.X, ItemListSize.Y));
						SlotPanel->SetPosition(FVector2D(ButtonSlot->GetPosition().X + ItemListPos.X, ButtonSlot->GetPosition().Y + ItemListPos.Y));
					}
				}
			}
		}
	}
}

void UEquipment::RemoveItemListWIdget(UCanvasPanel *InCanvas)
{
	for (auto CanvasElem : InCanvas->GetAllChildren())
	{
		if (CanvasElem == EquipmentContents)
		{
			InCanvas->RemoveChild(CanvasElem);
		}
	}
}

bool UEquipment::CanCheckCreateWidget(UCanvasPanel *InCanvas)
{
	bool bIsNothing = false;
	for (auto CanvasElem : InCanvas->GetAllChildren())
	{
		if (CanvasElem == EquipmentContents)
		{
			bIsNothing = false;
		}
		else
		{
			bIsNothing = true;
		}
	}
	return bIsNothing;
}

void UEquipment::EquipWeaponItem(UEquipmentSlot *SlotWidget)
{
	if (bIsActiveMainWeapon_BTN && SlotWidget)
	{
		MainWeaponSlot = SlotWidget;
		SetButtonStyle(MainWeaponSlot, MainWeaponButton);
		EquipMainWeapon();
	}
	if (bIsActiveSubWeapon_BTN && SlotWidget)
	{
		SubWeaponSlot = SlotWidget;
		SetButtonStyle(SubWeaponSlot, SubWeaponButton);
		EquipSubWeapon();
	}
	if (bIsActiveRangedWeapon_BTN && SlotWidget)
	{
		RangedWeaponSlot = SlotWidget;
		SetButtonStyle(RangedWeaponSlot, RangedWeaponButton);
		EquipRangedWeapon();
	}
}

void UEquipment::SetButtonStyle(UEquipmentSlot *SlotWidget, UButton *ToButton)
{
	if (ToButton)
	{
		FSlateBrush NormalBrush;
		NormalBrush.SetResourceObject(SlotWidget->GetWeaponItem()->GetItemTexture());
		NormalBrush.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); 

		FSlateBrush HoverBrush;
		HoverBrush.SetResourceObject(SlotWidget->GetWeaponItem()->GetItemTexture());
		HoverBrush.TintColor = FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);

		FSlateBrush PressedBrush;
		PressedBrush.SetResourceObject(SlotWidget->GetWeaponItem()->GetItemTexture());
		PressedBrush.TintColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);

		FButtonStyle ButtonStyle;
		ButtonStyle.Normal = NormalBrush;
		ButtonStyle.Hovered = HoverBrush;
		ButtonStyle.Pressed = PressedBrush;
		ToButton->SetStyle(ButtonStyle);
	}
}

void UEquipment::EquipMainWeapon()
{
	switch (MainWeaponSlot->GetWeaponItem()->GetWeaponType())
	{
		case EWeaponType::Katana:
		{
			WeaponComponent->EquipMainKatana();
		}
		break;
		case EWeaponType::Spear:
		{
			WeaponComponent->EquipMainSpear();
		}
		break;
		case EWeaponType::Bow:
		{
			WeaponComponent->EquipMainBow();
		}
		break;
		default:
		{
			break;
		}
	}
}

void UEquipment::EquipSubWeapon()
{
	switch (MainWeaponSlot->GetWeaponItem()->GetWeaponType())
	{
		case EWeaponType::Katana:
		{
		}
		break;
		case EWeaponType::Spear:
		{
		}
		break;
		case EWeaponType::Bow:
		{
		}
		break;
		default:
		{
			break;
		}
	}
}

void UEquipment::EquipRangedWeapon()
{
	switch (MainWeaponSlot->GetWeaponItem()->GetWeaponType())
	{
		case EWeaponType::Katana:
		{
		}
		break;
		case EWeaponType::Spear:
		{
		}
		break;
		case EWeaponType::Bow:
		{
		}
		break;
		default:
		{
			break;
		}
	}
}

void UEquipment::AddWeaponItem(UEquipmentSlot *SlotWidget)
{
	if (SlotWidget->GetWeaponItem())
	{
		OnAddWeaponItem.Broadcast(SlotWidget->GetWeaponItem());
	}
}

void UEquipment::RemoveWeaponItem(UEquipmentSlot *SlotWidget, int32 ItemIndex)
{
	OnRemoveWeaponItem.Broadcast(ItemIndex);
}
