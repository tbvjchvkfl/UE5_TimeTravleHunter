// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;
class APlayerCharacter;
class APlayerCharacterController;
class UPlayerAnimInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_TIMETRAVLEHUNTER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Weapon | WeaponStyle")
	TSubclassOf<AWeaponBase> KatanaWeapon;

	UPROPERTY(EditAnywhere, Category = "Weapon | WeaponStyle")
	TSubclassOf<AWeaponBase> SpearWeapon;

	UPROPERTY(EditAnywhere, Category = "Weapon | WeaponStyle")
	TSubclassOf<AWeaponBase> BowWeapon;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	UWeaponComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void EquipMainKatana();
	void EquipMainSpear();
	void EquipMainBow();
	void EquipSubKatana();
	void EquipSubSpear();
	void EquipRangedSpear();
	void EquipRangedBow();
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	AWeaponBase *Katana;

	UPROPERTY()
	AWeaponBase *Spear;

	UPROPERTY()
	AWeaponBase *Bow;

	UPROPERTY()
	APlayerCharacter *PlayerCharacter;

	UPROPERTY()
	APlayerCharacterController *PlayerCharacterController;

	UPROPERTY()
	UPlayerAnimInstance *PlayerCharacterAnimInstance;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
};
