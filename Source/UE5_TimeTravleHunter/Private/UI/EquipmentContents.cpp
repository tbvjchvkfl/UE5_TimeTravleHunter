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
	if (EquipWidget)
	{
		MainWeaponSlot = EquipWidget->GetMainWeaponSlot();
		SubWeaponSlot = EquipWidget->GetSubWeaponSlot();
		RangedWeaponSlot = EquipWidget->GetRangedWeaponSlot();
		bIsMainButtonState = EquipWidget->GetbIsButtonActiveMain();
		bIsSubButtonState = EquipWidget->GetbIsButtonActiveSub();
		bIsRangedButtonState = EquipWidget->GetbIsButtonActiveRanged();
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
	WidgetList.Empty();
	ContentsBox->ClearChildren();
	for (int32 i = 0; i < ListSize; i++)
	{
		if (EquipmentSlotWidget)
		{
			EquipmentSlot = CreateWidget<UEquipmentSlot>(GetOwningPlayer(), EquipmentSlotWidget);
			if (EquipmentSlot)
			{
				EquipmentSlot->OnAddWidget.AddUObject(this, &UEquipmentContents::AddWeaponWidget);
				EquipmentSlot->OnRemoveWidget.AddUObject(this, &UEquipmentContents::RemoveWeaponWidget);

				if (ItemList.IsValidIndex(i))
				{
					if (ItemList[i])
					{
						EquipmentSlot->InitializeEquipmentSlot(this, ItemList[i]);

						WidgetList.Add(EquipmentSlot);
					}
				}
				else
				{
					EquipmentSlot->InitializeEquipmentSlot(this, nullptr);
				}
				ContentsBox->AddChild(EquipmentSlot);
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
				if (ItemList.IsValidIndex(i))
				{
					GEngine->AddOnScreenDebugMessage(i + 200, 3, FColor::Black, FString("Is Valid"));
				}
				//EquipmentSlot->InitializeEquipmentSlot(this, EquipWidget);
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
			return;
		}
	}
}

void UEquipmentContents::AddWeaponWidget(UEquipmentSlot *SlotWidget)
{
	OnAddWeaponWidget.Broadcast(SlotWidget);
	RefreshEquipmentSlot();
}
