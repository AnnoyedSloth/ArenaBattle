// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPawn.h"


// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	RootComponent = capsule;
	mesh->SetupAttachment(capsule);
	springArm->SetupAttachment(capsule);
	camera->SetupAttachment(springArm);

	capsule->SetCapsuleHalfHeight(88.0f);
	capsule->SetCapsuleRadius(34.0f);
	mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	springArm->TargetArmLength = 400.0f;
	springArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MESH(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Standard.SK_CharM_Standard"));
	if (SK_MESH.Succeeded())
	{
		mesh->SetSkeletalMesh(SK_MESH.Object);
	}
	
	

}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AABPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AABPawn::LeftRight);
}

void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//ABLOG_S(Warning)
}

void AABPawn::PossessedBy(AController* controller)
{
	//ABLOG_S(Warning)
	Super::PossessedBy(controller);
}

void AABPawn::UpDown(float value)
{
	//ABLOG(Warning, TEXT("%f"), value)
	AddMovementInput(GetActorForwardVector(), value);
}

void AABPawn::LeftRight(float value)
{
	//ABLOG(Warning, TEXT("%f"), value)
	AddMovementInput(GetActorRightVector(), value);
}