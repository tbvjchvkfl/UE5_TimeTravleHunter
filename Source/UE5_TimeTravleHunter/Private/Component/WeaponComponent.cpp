// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Component/WeaponComponent.h"
#include "Character/Armor/WeaponBase.h"
#include "Character/Player/PlayerCharacter.h"


// Engine


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::EquipKatana()
{
	if (KatanaWeapon)
	{
		Katana = GetWorld()->SpawnActor<AWeaponBase>(KatanaWeapon);
		Katana->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WeaponSocket_Katana");
	}
}

void UWeaponComponent::EquipSpear()
{
	if (SpearWeapon)
	{
		Spear = GetWorld()->SpawnActor<AWeaponBase>(KatanaWeapon);
		Spear->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WeaponSocket_Spear");
	}
}

void UWeaponComponent::EquipBow()
{
	if (BowWeapon)
	{
		Bow = GetWorld()->SpawnActor<AWeaponBase>(KatanaWeapon);
		Bow->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "MainWeapon_Bow");
	}
}

