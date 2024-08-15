// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class UE5_TIMETRAVLEHUNTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//======================================================
	//=					- Variables -					   =
	//======================================================



	//======================================================
	//=					- Functionary -					   =
	//======================================================
	APlayerCharacter();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth() { return MaxHealth; };

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	// Component
	UPROPERTY(EditAnywhere, Category = "Character | Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *MainCamera;

	UPROPERTY(EditAnywhere, Category = "Character | Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *CameraBoom;



	// Input
	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext *DefaultContext;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction *Moving;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction *Jumping;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction *Looking;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// InputFunction
	void Move(const struct FInputActionValue &Value);
	void Look(const struct FInputActionValue &Value);
	void Sprint(const struct FInputActionValue &Value);


private:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float AbilityStamina;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	

};
