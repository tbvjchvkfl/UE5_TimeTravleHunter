// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class UE5_TIMETRAVLEHUNTER_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	//======================================================
	//=					- Variables -					   =
	//======================================================
	

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	AWeaponBase();

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Weapon | Base Data")
	UStaticMeshComponent *WeaponMesh;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
