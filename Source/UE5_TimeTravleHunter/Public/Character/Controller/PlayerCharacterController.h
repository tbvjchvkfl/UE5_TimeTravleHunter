// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class APlayerCharacter;
class ATTH_HUD;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Controller | Base")
	TSubclassOf<APlayerCharacter>OwnerCharacter;

	UPROPERTY(EditAnywhere, Category = "Controller | Property")
	float LookingRotationValue;

	// Character Locomotion
	UPROPERTY(VisibleAnywhere, Category = "Controller | Status")
	bool bIsJog;

	UPROPERTY(VisibleAnywhere, Category = "Controller | Status")
	bool bIsSprint;

	UPROPERTY(VisibleAnywhere, Category = "Controller | Status")
	bool bIsCrouch;

	UPROPERTY(VisibleAnywhere, Category = "Controller | Status")
	bool bIsAimming;

	UPROPERTY(VisibleAnywhere, Category = "Controller | Status")
	bool bIsParkour;

	UPROPERTY(VisibleAnywhere, Category = "Controller | Status")
	bool bIsKatanaState;

	UPROPERTY(VisibleAnywhere, Category = "Controller | Status")
	bool bIsSpearState;

	UPROPERTY(VisibleAnywhere, Category = "Controller | Status")
	bool bIsBowState;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	APlayerCharacterController();

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================



	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(VisibleAnywhere, Category = "Controller | Property", meta = (AllowPrivateAccess = "true"))
	APlayerCharacter *PlayerCharacter;

	UPROPERTY()
	ATTH_HUD *OwningHUD;

	//======================================================
	//=					- Functionary -					   =
	//======================================================

};
