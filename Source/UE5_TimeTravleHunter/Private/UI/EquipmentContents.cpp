// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "UI/EquipmentContents.h"
#include "UI/EquipmentSlot.h"
#include "UI/Equipment.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/InventoryComponent.h"

// Engine
#include "Components/ScrollBox.h"

void UEquipmentContents::InitializeEquipmentContents(UEquipment* EquipmentWidget)
{
	if (EquipmentWidget)
	{
		EquipWidget = EquipmentWidget;
		MainWeaponSlot = EquipmentWidget->GetMainWeaponSlot();
		SubWeaponSlot = EquipmentWidget->GetSubWeaponSlot();
		RangedWeaponSlot = EquipmentWidget->GetRangedWeaponSlot();
		bIsMainButtonState = EquipmentWidget->GetbIsButtonActiveMain();
		bIsSubButtonState = EquipmentWidget->GetbIsButtonActiveSub();
		bIsRangedButtonState = EquipmentWidget->GetbIsButtonActiveRanged();
	}
	RefreshEquipmentSlot();
	OnEquipWeaponWidget.AddUObject(this, &UEquipmentContents::EquipWeaponWidget);
}

void UEquipmentContents::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	for (int32 i = 0; i < WidgetList.Num(); i++)
	{
		if (WidgetList[i]->GetWeaponItem())
		{
			GEngine->AddOnScreenDebugMessage(i + 20, 3, FColor::Blue, FString::Printf(TEXT("Index %d : True"), i));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(i + 20, 3, FColor::Blue, FString::Printf(TEXT("Index %d : False"), i));
		}
	}
	for (int32 i = 0; i < ItemList.Num(); i++)
	{
		if (ItemList[i])
		{
			GEngine->AddOnScreenDebugMessage(i + 40, 3, FColor::Blue, FString::Printf(TEXT("Index %d : Item"), i));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(i + 40, 3, FColor::Blue, FString::Printf(TEXT("Index %d : Null"), i));
		}
	}
}

void UEquipmentContents::InitEssentialData()
{
	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComponent = Player->GetItemInventory();
		if (InventoryComponent)
		{
			ItemList = InventoryComponent->GetWeaponInventory();
			ListSize = InventoryComponent->GetWeaponInventorySize();

			InventoryComponent->OnInventoryUpdate.AddUObject(this, &UEquipmentContents::RefreshEquipmentSlot);
		}
	}
}

void UEquipmentContents::RefreshEquipmentSlot()
{
	InitEssentialData();
	FillEmptySlot();
	for (int32 i = 0; i < ItemList.Num(); i++)
	{
		if (ItemList[i])
		{
			if (WidgetList[i] && !WidgetList[i]->GetWeaponItem())
			{
				WidgetList[i]->SetItemInfo(ItemList[i]);
			}
		}
	}
}

void UEquipmentContents::FillEmptySlot()
{
	WidgetList.Empty();
	ContentsBox->ClearChildren();
	for (int32 i = 0; i < ListSize; i++)
	{
		if (EquipmentSlotWidget)
		{
			EquipmentSlot = CreateWidget<UEquipmentSlot>(GetOwningPlayer(), EquipmentSlotWidget);

			if (EquipmentSlot)
			{
				EquipmentSlot->InitializeEquipmentSlot(this);
				EquipmentSlot->OnAddWidget.AddUObject(this, &UEquipmentContents::AddWeaponWidget);
				EquipmentSlot->OnRemoveWidget.AddUObject(this, &UEquipmentContents::RemoveWeaponWidget);

				WidgetList.Add(EquipmentSlot);
				ContentsBox->AddChild(EquipmentSlot);
			}
		}
	}
}

void UEquipmentContents::EquipWeaponWidget(UEquipmentSlot *SlotWidget)
{
	EquipWidget->OnEquipWeapon.Broadcast(SlotWidget);
}

void UEquipmentContents::RemoveWeaponWidget(UEquipmentSlot *SlotWidget)
{
	for (int32 i = 0; i < ItemList.Num(); i++)
	{
		if (ItemList[i] == SlotWidget->GetWeaponItem())
		{
			ItemList.RemoveAt(i);
			ContentsBox->RemoveChildAt(i);
			OnRemoveWeaponWidget.Broadcast(SlotWidget, i);
			GEngine->AddOnScreenDebugMessage(82, 3, FColor::Green, FString("Remove!!!"));
			return;
		}
	}
}

void UEquipmentContents::AddWeaponWidget(UEquipmentSlot *SlotWidget)
{
	OnAddWeaponWidget.Broadcast(SlotWidget);
	ItemList.Add(SlotWidget->GetWeaponItem());

	GEngine->AddOnScreenDebugMessage(83, 3, FColor::Green, FString("Add!!!"));
}
