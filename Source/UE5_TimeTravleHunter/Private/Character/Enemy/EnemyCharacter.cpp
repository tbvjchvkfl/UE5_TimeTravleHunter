// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/AnimInstance/EnemyAnimInstance.h"
#include "Character/AnimInstance/PlayerAnimInstance.h"
#include "Character/Controller/EnemyCharacterController.h"
#include "Character/Armor/WeaponBase.h"

// Engine
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter() : bIsTargetDetecting(false)
{
	PrimaryActorTick.bCanEverTick = true;
	AssasinationCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AssasinationColl"));
	AssasinationCollision->SetupAttachment(RootComponent);
	AssasinationPosMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AssasinationMesh"));
	AssasinationPosMesh->SetupAttachment(RootComponent);

	DetectingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DetectingWidget"));
	DetectingWidget->SetupAttachment(RootComponent);
	DetectingWidget->SetDrawSize(FVector2D(50.0f, 100.0f));
	DetectingWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	DetectingWidget->SetVisibility(false);

	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidget"));
	LockOnWidget->SetupAttachment(RootComponent);
	LockOnWidget->SetDrawSize(FVector2D(15.0f, 15.0f));
	LockOnWidget->SetRelativeLocation(FVector(20.0f, 0.0f, 20.0f));
	LockOnWidget->SetVisibility(false);

	static ConstructorHelpers::FClassFinder<AEnemyCharacterController>EnemyControllerClass(TEXT("/Game/TimeTravleHunter/Controller/BP_EnemyCharacterController.BP_EnemyCharacterController_C"));
	if (EnemyControllerClass.Succeeded())
	{
		AIControllerClass = EnemyControllerClass.Class;
	}
	else
	{
		AIControllerClass = AEnemyCharacterController::StaticClass();
	}
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bIsAssasinationable = false;
	bIsTargetDetecting = true;
	bIsLockFromTarget = false;
	TimeForDetectIcon = 2.0f;
	GetCharacterMovement()->GravityScale = 200.0f;
}

void AEnemyCharacter::ActiveStrafe(bool ActivationValue)
{
	if (OwningController)
	{
		if (ActivationValue)
		{
			OwningController->SetFocus(TargetCharacter);
			OwningController->bIsStrafe = true;
		}
		else
		{
			OwningController->SetFocus(nullptr);
			OwningController->bIsStrafe = false;
		}
	}
}

FVector AEnemyCharacter::StrafeMovement()
{
	FVector DestLoc = FVector::ZeroVector;
	float RandomValue = FMath::FRand();
	int32 SelectedDirIndex = RandomValue < 0.5f ? 0 : 1;
	FVector CharacterRightVector = GetActorRightVector() * 200.0f;
	if (SelectedDirIndex == 0)
	{
		DestLoc = GetActorLocation() + CharacterRightVector;
	}
	if (SelectedDirIndex == 1)
	{
		DestLoc = GetActorLocation() - CharacterRightVector;
	}
	return DestLoc;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	TargetCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	OwningAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	OwningController = Cast<AEnemyCharacterController>(GetController());
	if (OwningController)
	{
		OwningController->OnDetecting.AddUObject(this, &AEnemyCharacter::ToggleDetectIcon);
	}

	EquipWeapon();

	AssasinationCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapBegin);
	AssasinationCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapEnd);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!OwningController)
	{
		OwningController = Cast<AEnemyCharacterController>(GetController());
	}
	if (OwningController)
	{
		ModifyMovementSpeed();
		ToggleDetectIcon();
		RotationDetectIcon();
		ToggleLockIcon(bIsLockFromTarget);
	}
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
	OwningAnimInstance->PlayAssasination_Vic();
	GetWorld()->GetTimerManager().SetTimer(RagDollTimer, this, &AEnemyCharacter::RagDoll, OwningAnimInstance->Assasination_Vic->GetPlayLength(), false);
	Location = AssasinationPosMesh->GetComponentLocation();
	Rotation = GetActorRotation();
}

void AEnemyCharacter::RagDoll()
{
	GetMesh()->SetSimulatePhysics(true);
}

void AEnemyCharacter::ToggleDetectIcon()
{
	if (OwningController->bIsDetecting && DetectingWidget)
	{
		if (bIsTargetDetecting)
		{
			DetectingWidget->SetVisibility(true);
			GetWorld()->GetTimerManager().SetTimer(WidgetVisibilityTimer, [&]()
				{
					DetectingWidget->SetVisibility(false);
					GetWorld()->GetTimerManager().ClearTimer(WidgetVisibilityTimer);
				}, TimeForDetectIcon, false);
		}
		bIsTargetDetecting = false;
	}
}

void AEnemyCharacter::RotationDetectIcon()
{
	if (TargetCharacter && DetectingWidget)
	{
		FVector PlayerLocation = TargetCharacter->GetActorLocation();
		FVector Direction = PlayerLocation - GetActorLocation();
		FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		DetectingWidget->SetWorldRotation(NewRotation);
	}
}

void AEnemyCharacter::ModifyMovementSpeed()
{
	if (OwningController->bIsRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	}
}

void AEnemyCharacter::EquipWeapon()
{
	if (WeaponBaseClass)
	{
		WeaponBase = GetWorld()->SpawnActor<AWeaponBase>(WeaponBaseClass);
		WeaponBase->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "EnemyWeaponSocket");
	}
}

void AEnemyCharacter::ToggleLockIcon(bool CheckInput)
{
	if (LockOnWidget)
	{
		if (CheckInput)
		{
			LockOnWidget->SetVisibility(true);
		}
		else
		{
			LockOnWidget->SetVisibility(false);
		}
	}
}
