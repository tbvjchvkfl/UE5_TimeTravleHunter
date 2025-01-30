// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CharacterActionInterface.h"
#include "EnemyCharacter.generated.h"

class USphereComponent;
class APlayerCharacter;
class UEnemyAnimInstance;
class AEnemyCharacterController;
class UWidgetComponent;
class AWeaponBase;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API AEnemyCharacter : public ACharacter, public ICharacterActionInterface
{
	GENERATED_BODY()

public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	FTimerHandle RagDollTimer;
	FTimerHandle WidgetVisibilityTimer;

	UPROPERTY()
	bool bIsAssasinationable;

	UPROPERTY()
	bool bIsTargetDetecting;

	UPROPERTY()
	bool bIsLockFromTarget;

	

	UPROPERTY(EditAnywhere, Category = "Character | Weapon")
	TSubclassOf<AWeaponBase> WeaponBaseClass;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	AEnemyCharacter();
	void ActiveStrafe(bool ActivationValue);
	FVector StrafeMovement();

	FORCEINLINE APlayerCharacter *GetTargetCharacter()const { return TargetCharacter; };
protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Character | Component")
	USphereComponent *AssasinationCollision;

	UPROPERTY(EditAnywhere, Category = "Character | Component")
	UWidgetComponent *DetectingWidget;

	UPROPERTY(EditAnywhere, Category = "Character | Component")
	UWidgetComponent *LockOnWidget;

	UPROPERTY(VisibleAnywhere, Category = "Character | Property")
	APlayerCharacter *TargetCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Character | Property")
	UEnemyAnimInstance *OwningAnimInstance;

	UPROPERTY(VisibleAnywhere, Category = "Character | Property")
	AEnemyCharacterController *OwningController;

	UPROPERTY(EditAnywhere, Category = "Character | Property")
	float TimeForDetectIcon;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Character | ActionReference", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent *AssasinationPosMesh;

	UPROPERTY()
	AWeaponBase *WeaponBase;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void StealthAssain(FVector &Location, FRotator &Rotation) override;
	void RagDoll();
	void ToggleDetectIcon();
	void RotationDetectIcon();
	void ModifyMovementSpeed();
	void EquipWeapon();
	void ToggleLockIcon(bool CheckInput);
};
