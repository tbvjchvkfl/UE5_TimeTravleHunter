// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Equipment.generated.h"

class UButton;
class UScrollBox;
class UTextBlock;
class UCanvasPanel;
class UBorder;
class APlayerCharacter;
class UInventoryComponent;
class UWeaponComponent;
class UEquipmentSlot;
class UEquipmentContents;
class APickUpItem;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipWeapon, UEquipmentSlot *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddWeaponItem, APickUpItem *);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemoveWeaponItem, int32);

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UEquipment : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FOnEquipWeapon OnEquipWeapon;
	FOnAddWeaponItem OnAddWeaponItem;
	FOnRemoveWeaponItem OnRemoveWeaponItem;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *MainWeaponCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *SubWeaponCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel *RangedWeaponCanvas;

	UPROPERTY(meta = (BindWidget))
	UBorder *MainWeaponBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder *SubWeaponBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder *RangedWeaponBorder;

	/*UPROPERTY(meta = (BindWidget))
	UEquipmentContents *MainEquipmentContents;

	UPROPERTY(meta = (BindWidget))
	UEquipmentContents *SubEquipmentContents;

	UPROPERTY(meta = (BindWidget))
	UEquipmentContents *RangedEquipmentContents;*/

	UPROPERTY(meta = (BindWidget))
	UButton *MainWeaponButton;

	UPROPERTY(meta = (BindWidget))
	UButton *SubWeaponButton;

	UPROPERTY(meta = (BindWidget))
	UButton *RangedWeaponButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *WeaponName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *WeaponDescription;

	UPROPERTY(EditAnywhere, Category = "Equipment | Widget")
	TSubclassOf<UEquipmentContents> EquipmentContentsWidget;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	FORCEINLINE UEquipmentSlot *GetMainWeaponSlot()const { return MainWeaponSlot; };
	FORCEINLINE UEquipmentSlot *GetSubWeaponSlot()const { return SubWeaponSlot; };
	FORCEINLINE UEquipmentSlot *GetRangedWeaponSlot()const { return RangedWeaponSlot; };
	FORCEINLINE bool GetbIsButtonActiveMain()const { return bIsActiveMainWeapon_BTN; };
	FORCEINLINE bool GetbIsButtonActiveSub()const { return bIsActiveSubWeapon_BTN; };
	FORCEINLINE bool GetbIsButtonActiveRanged()const { return bIsActiveRangedWeapon_BTN; };

	void InitializeEquipment();
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	APlayerCharacter *Player;

	UPROPERTY()
	UWeaponComponent *WeaponComponent;

	UPROPERTY()
	UEquipmentContents *EquipmentContents;

	UPROPERTY()
	UEquipmentSlot *MainWeaponSlot;

	UPROPERTY()
	UEquipmentSlot *SubWeaponSlot;

	UPROPERTY()
	UEquipmentSlot *RangedWeaponSlot;

	UPROPERTY(EditAnywhere, Category = "Equipment | Property", meta = (AllowPrivateAccess = "true"))
	FVector2D ItemListSize;

	UPROPERTY(EditAnywhere, Category = "Equipment | Property", meta = (AllowPrivateAccess = "true"))
	FVector2D ItemListPos;

	UPROPERTY()
	bool bIsActiveMainWeapon_BTN;

	UPROPERTY()
	bool bIsActiveSubWeapon_BTN;

	UPROPERTY()
	bool bIsActiveRangedWeapon_BTN;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	UFUNCTION()
	void OnMainWeaponClicked();
	UFUNCTION()
	void OnSubWeaponClicked();
	UFUNCTION()
	void OnRangedWeaponClicked();

	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent);
	void ButtonInteraction(UCanvasPanel* InCanvas, UBorder *InBorder);
	void RemoveItemListWIdget(UCanvasPanel *InCanvas);
	bool CanCheckCreateWidget(UCanvasPanel *InCanvas);
	void EquipWeaponItem(UEquipmentSlot *SlotWidget);
	void SetButtonStyle(UEquipmentSlot *SlotWidget, UButton *ToButton);
	void EquipMainWeapon();
	void EquipSubWeapon();
	void EquipRangedWeapon();
	void AddWeaponItem(UEquipmentSlot* SlotWidget);
	void RemoveWeaponItem(UEquipmentSlot *SlotWidget, int32 ItemIndex);
};
