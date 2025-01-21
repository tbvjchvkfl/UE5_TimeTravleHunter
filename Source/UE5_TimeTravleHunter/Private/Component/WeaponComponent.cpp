// Fill out your copyright notice in the Description page of Project Settings.


// GameFramework
#include "Component/WeaponComponent.h"
#include "Character/Armor/WeaponBase.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/AnimInstance/PlayerAnimInstance.h"
#include "Character/Controller/PlayerCharacterController.h"

// Engine


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	PlayerCharacterController = Cast<APlayerCharacterController>(PlayerCharacter->GetController());
	PlayerCharacterAnimInstance = Cast<UPlayerAnimInstance>(PlayerCharacter->GetMesh()->GetAnimInstance());
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::EquipMainKatana()
{
	if (KatanaWeapon)
	{
		Katana = GetWorld()->SpawnActor<AWeaponBase>(KatanaWeapon);
		Katana->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WeaponSocket_Katana");
		PlayerCharacterAnimInstance->PlayEquipKatana();
		PlayerCharacter->KatanaCoverMesh->SetVisibility(true);
		PlayerCharacterController->bIsKatanaState = true;
		PlayerCharacterController->bIsSpearState = false;
	}
}

void UWeaponComponent::EquipMainSpear()
{
	if (SpearWeapon)
	{
		Spear = GetWorld()->SpawnActor<AWeaponBase>(KatanaWeapon);
		Spear->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WeaponSocket_Spear");

		PlayerCharacterController->bIsKatanaState = false;
		PlayerCharacterController->bIsSpearState = true;
	}
}

void UWeaponComponent::EquipMainBow()
{
	if (BowWeapon)
	{
		Bow = GetWorld()->SpawnActor<AWeaponBase>(KatanaWeapon);
		Bow->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "MainWeapon_Bow");
	}
}

void UWeaponComponent::EquipSubKatana()
{
}

void UWeaponComponent::EquipSubSpear()
{
}

void UWeaponComponent::EquipRangedSpear()
{
}

void UWeaponComponent::EquipRangedBow()
{
}

