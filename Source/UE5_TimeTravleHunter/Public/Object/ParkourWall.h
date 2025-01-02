// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParkourWall.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API AParkourWall : public AActor
{
	GENERATED_BODY()
	
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================


	//======================================================
	//=					- Functionary -					   =
	//======================================================
	AParkourWall();

	FORCEINLINE FVector GetWallSize()const { return WallMesh->GetComponentTransform().GetScale3D(); };

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Wall | Component")
	UStaticMeshComponent *WallMesh;

	UPROPERTY(EditAnywhere, Category = "Wall | Component")
	UBoxComponent *CollisionBox;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
};
