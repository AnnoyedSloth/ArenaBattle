// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"
#include "ABAnimInstance.h"


// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	springArm->TargetArmLength = 400.0f;
	springArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MESH(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Standard.SK_CharM_Standard"));
	if (SK_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MESH.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		WARRIOR_ANIM(TEXT("/Game/Animation/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	SetControlMode(curControlMode);

	armLengthSpeed = 3.0f;
	armRotationSpeed = 10.0f;
	
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	isAttacking = false;
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	springArm->TargetArmLength = FMath::FInterpTo(springArm->TargetArmLength, armLengthTo, DeltaTime, armLengthSpeed);


	switch (curControlMode)
	{
	case EControlMode::DIABLO:
		springArm->RelativeRotation = FMath::RInterpTo(springArm->RelativeRotation, armRotationTo, DeltaTime, armRotationSpeed);
		if (directionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(directionToMove).Rotator());
			AddMovementInput(directionToMove);
		}
		break;
	}
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// AnimInstance의 포인터는 PostInitializeComponents 함수에서 불러오자
	animInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());

	// 동적으로 델리게이트 함수를 추가하는것 또한 이곳에서..
	animInstance->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AABCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AABCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
	
	PlayerInputComponent->BindAction(TEXT("ChangeMode"), IE_Pressed, this, &AABCharacter::ChangeMode);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AABCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AABCharacter::Attack);
}

void AABCharacter::MoveForward(float value)
{
	switch (curControlMode)
	{
	case EControlMode::GTA:
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), value);
	break;

	case EControlMode::DIABLO:
		directionToMove.X = value;	
	break;
	}

}

void AABCharacter::MoveRight(float value)
{
	switch (curControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), value);
		break;

	case EControlMode::DIABLO:
		directionToMove.Y = value;
		break;
	}
}

void AABCharacter::LookUp(float value)
{
	switch (curControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(value);
		break;
	}
}

void AABCharacter::Turn(float value)
{
	switch (curControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(value);
		break;
	}
}

void AABCharacter::SetControlMode(EControlMode controlMode)
{

	curControlMode = controlMode;

	switch (curControlMode)
	{
	case EControlMode::GTA:
		//springArm->TargetArmLength = 450.0f;
		//springArm->SetRelativeRotation(FRotator::ZeroRotator);
		armLengthTo = 450.0f;
		springArm->bUsePawnControlRotation = true;
		springArm->bInheritPitch = true;
		springArm->bInheritRoll = true;
		springArm->bInheritYaw = true;
		springArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	
	case EControlMode::DIABLO:
		//springArm->TargetArmLength = 800.0f;
		//springArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		armLengthTo = 800.0f;
		armRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		springArm->bUsePawnControlRotation = false;
		springArm->bInheritPitch = false;
		springArm->bInheritRoll = false;
		springArm->bInheritYaw = false;
		springArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		break;
	}
}

void AABCharacter::ChangeMode()
{
	if (curControlMode == EControlMode::DIABLO)
	{
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::GTA);
	}
	else
	{
		GetController()->SetControlRotation(springArm->RelativeRotation);
		SetControlMode(EControlMode::DIABLO);
	}
}

void AABCharacter::Attack()
{
	if (isAttacking) return;
	if (!animInstance) return;

	animInstance->PlayAttackMontage();
	isAttacking = true;
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* montage, bool isInterrupted)
{
	isAttacking = false;
}