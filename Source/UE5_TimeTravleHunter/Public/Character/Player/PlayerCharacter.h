// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

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
	UCameraComponent *MainCamera;

	UPROPERTY(EditAnywhere, Category = "Character | Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *CameraBoom;



	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *DefaultContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Moving;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Jumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Looking;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Sprinting;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// InputFunction
	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	void Sprint(const FInputActionValue &Value);


private:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float AbilityStamina;

	// 값의 범위는 0 ~ 1
	UPROPERTY(EditAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float LookingRotationValue;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	

};
