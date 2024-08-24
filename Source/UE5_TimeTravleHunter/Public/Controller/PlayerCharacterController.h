// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class APlayerCharacter;


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
	UPROPERTY(VisibleAnywhere, Category = "Controller | Status", meta = (AllowPrivateAccess = "true"))
	APlayerCharacter *PlayerCharacter;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	
};
