// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ItemStruct.h"
#include "Interface/InteractionInterface.h"
#include "PickUpItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class AWeaponBase;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API APickUpItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	//======================================================
	//=					- Variables -					   =
	//======================================================
	

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	APickUpItem();
	FORCEINLINE EItemType GetItemType() const { return ItemType; };
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; };
	FORCEINLINE int32 GetCurrentQuantity() const { return CurrentQuantity; };
	FORCEINLINE int32 GetMaxQuantity() const { return MaxQuantity; };
	FORCEINLINE FIntPoint GetItemSize() const { return ItemSize; };
	FORCEINLINE int32 GetItemNumber() const { return ItemNumber; };
	FORCEINLINE UTexture2D* GetItemTexture() const { return ItemTexture; };
	FORCEINLINE float GetItemRotation() const { return ItemRotation; };
	FORCEINLINE FName GetItemRowName() const { return ItemRowName; };

	FORCEINLINE int32 SetCurrentQuantity(int32 NewQuantity) { return CurrentQuantity = FMath::Clamp(NewQuantity, 0, MaxQuantity); };
	FORCEINLINE float SetItemRotation(float NewRotateValue) { return ItemRotation = FMath::Clamp(NewRotateValue, 0.0f, 90.0f); };

	TArray<FVector2D> GetShape(float Rotation) const;
	FIntPoint GetMaxSize(float Rotation, bool DefaultOverride);

	void SetActivateItem(bool ActivationState);
	void SetItemData(FName ItemRow, FItemData *ItemData);
	
protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	// Item Component
	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | Component")
	UStaticMeshComponent *ItemMesh;

	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | Component")
	USphereComponent *CollisionSphere;

	// Item Data
	UPROPERTY(EditAnywhere, Category = "ItemBaseInfo | ItemData")
	UDataTable *ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "ItemBaseInfo | ItemData")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | ItemData")
	FName ItemRowName;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Consumable")
	int32 ItemNumber;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Consumable")
	FIntPoint ItemSize;

	UPROPERTY(EditInstanceOnly, Category = "ItemBaseInfo | Consumable")
	int32 CurrentQuantity;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Consumable")
	int32 MaxQuantity;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Consumable")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Consumable")
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Consumable")
	FString ItemName;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Consumable")
	FString ItemDescription;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Consumable")
	UTexture2D *ItemTexture;

	// Item Property
	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Item Property")
	TArray<FVector2D> ItemShape;

	UPROPERTY(VisibleAnywhere, Category = "ItemBaseInfo | Item Property")
	float ItemRotation;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void InitializeItemData();
	void DesiredItemShape();
	virtual void PickUpInteraction(class APlayerCharacter *Player) override;
	


	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
