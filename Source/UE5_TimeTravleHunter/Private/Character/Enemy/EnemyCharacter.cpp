// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/Player/PlayerCharacter.h"
#include "AnimInstance/EnemyAnimInstance.h"

// Engine
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AssasinationCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AssasinationColl"));
	AssasinationCollision->SetupAttachment(RootComponent);
	AssasinationPosMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AssasinationMesh"));
	AssasinationPosMesh->SetupAttachment(RootComponent);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	TargetCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	EOwningAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	AssasinationCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapBegin);
	AssasinationCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapEnd);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (auto Target = Cast<APlayerCharacter>(OtherActor))
	{
		bIsAssasinationable = true;
	}
}

void AEnemyCharacter::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (auto Target = Cast<APlayerCharacter>(OtherActor))
	{
		bIsAssasinationable = true;
	}
}

void AEnemyCharacter::StealthAssain(FVector &Location, FRotator &Rotation)
{
	EOwningAnimInstance->PlayAssasination_Vic();
	GetWorld()->GetTimerManager().SetTimer(RagDollTimer, this, &AEnemyCharacter::RagDoll, EOwningAnimInstance->Assasination_Vic->GetPlayLength(), false);
	Location = AssasinationPosMesh->GetComponentLocation();
	Rotation = GetActorRotation();
}

void AEnemyCharacter::RagDoll()
{
	GetMesh()->SetSimulatePhysics(true);
}