// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CharacterActionInterface.h"
#include "EnemyCharacter.generated.h"

class USphereComponent;
class APlayerCharacter;
class UEnemyAnimInstance;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API AEnemyCharacter : public ACharacter, public ICharacterActionInterface
{
	GENERATED_BODY()

public:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	bool bIsAssasinationable;

	FTimerHandle RagDollTimer;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	AEnemyCharacter();
	

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Character | Component")
	USphereComponent *AssasinationCollision;

	UPROPERTY(VisibleAnywhere, Category = "Character | Property")
	APlayerCharacter *TargetCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Character | Property")
	UEnemyAnimInstance *EOwningAnimInstance;

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

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void StealthAssain(FVector &Location, FRotator &Rotation) override;
	void RagDoll();
};
