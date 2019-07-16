// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "ABWeapon.h"
#include "ABAIController.h"
#include "ABCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"
#include "DrawDebugHelpers.h"
#include "ABCharacterSetting.h"
#include "ABGameInstance.h"
#include "ABPlayerController.h"


// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	characterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	hpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	//AIControllerClass = AABPlayerController::StaticClass();
	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Custum ini reading line
	//auto defaultSetting = GetDefault<UABCharacterSetting>();
	//if (defaultSetting->characterAssets.Num() > 0)
	//{
	//	for (auto characterAsset : defaultSetting->characterAssets)
	//	{
	//		ABLOG(Warning, TEXT("Character Asset : %s"), *characterAsset.ToString());
	//	}
	//}
	//

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MESH(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Standard.SK_CharM_Standard"));
	if (SK_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MESH.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		WARRIOR_ANIM(TEXT("/Game/Animation/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		hpBarWidget->SetWidgetClass(UI_HUD.Class);
		hpBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);
	hpBarWidget->SetupAttachment(GetMesh());
	   
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	springArm->TargetArmLength = 400.0f;
	springArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	hpBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	hpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	SetControlMode(curControlMode);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);


	// 카메라 모드 변경시 길이/각 보간 속도
	armLengthSpeed = 3.0f;
	armRotationSpeed = 10.0f;
	
	//점프높이
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	isAttacking = false;

	maxCombo = 4;
	// 콤보 값에 대해 초기화 하기 위해서
	AttackEndComboState();

	// 충돌 프리셋 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// 공격범위
	attackRange = 100.0f;
	attackRadius = 100.0f;

	assetIndex = 4;

	SetActorHiddenInGame(true);
	hpBarWidget->SetHiddenInGame(true);
	bCanBeDamaged = false;

	deadTimer = 5.0f;

}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	//FName weaponSocket(TEXT("hand_rSocket"));
	//AABWeapon* curWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//if (curWeapon != nullptr)
	//{
	//	curWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weaponSocket);
	//}

	isPlayer = IsPlayerControlled();

	if (isPlayer)
	{
		playerController = Cast<AABPlayerController>(GetController());
		ABCHECK(playerController);
	}
	else
	{
		aiController = Cast<AABAIController>(GetController());
		ABCHECK(aiController);
	}

	auto defaultSetting = GetDefault<UABCharacterSetting>();

	if (isPlayer)
	{
		assetIndex = 4;
	}
	else
	{
		assetIndex = FMath::RandRange(0, defaultSetting->characterAssets.Num() - 1);
	}

	characterAssetToLoad = defaultSetting->characterAssets[assetIndex];
	
	auto gameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(gameInstance);

	assetStreamingHandle = gameInstance->streamableManager.RequestAsyncLoad(characterAssetToLoad,
		FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);

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

void AABCharacter::SetCharacterState(ECharacterState newState)
{
	ABCHECK(currentState != newState);
	currentState = newState;

	switch (currentState)
	{
	case ECharacterState::LOADING:
		if (isPlayer)
		{
			DisableInput(playerController);
		}
		SetActorHiddenInGame(true);
		hpBarWidget->SetHiddenInGame(true);
		bCanBeDamaged = false;
		break;
	case ECharacterState::READY:
		SetActorHiddenInGame(false);
		hpBarWidget->SetHiddenInGame(false);
		bCanBeDamaged = true;

		characterStat->onHPIsZero.AddLambda([this]()-> void{
			SetCharacterState(ECharacterState::DEAD);
		});

		auto characterWidget = Cast<UABCharacterWidget>(hpBarWidget->GetUserWidgetObject());
		ABCHECK(characterWidget);
		characterWidget->BindCharacterStat(characterStat);

		if (isPlayer)
		{
			SetControlMode(EControlMode::DIABLO);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(playerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			aiController->RunAI();
		}

		break;
	case ECharacterState::DEAD:
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		hpBarWidget->SetHiddenInGame(true);
		animInstance->SetDeadAnim();
		bCanBeDamaged = false;
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

	//ABLOG(Warning, TEXT("PostInitializeComponents Called and Start"))
	// 람다식으로 델리게이트 로직 등록
	animInstance->onNextAttackCheck.AddLambda([this]() -> void {

		//ABLOG(Warning, TEXT("OnNextAttackCheck"))
		canNextCombo = false;

		if (isComboInputOn)
		{
			//ABLOG(Warning, TEXT("currentCombo = %d"), currentCombo);
			AttackStartComboState();
			animInstance->JumpToAttackMontageSection(currentCombo);
		}
	});

	animInstance->onAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	characterStat->onHPIsZero.AddLambda([this]()->void{
		ABLOG(Warning, TEXT("OnHPIsZero"));
		animInstance->SetDeadAnim();
		SetActorEnableCollision(false);
	});

	UABCharacterWidget* characterWidget = 
		Cast<UABCharacterWidget>(hpBarWidget->GetUserWidgetObject());


	if (characterWidget)
	{
		//ABLOG(Warning, TEXT("Character Widget Casting Succeeded"));
		characterWidget->BindCharacterStat(characterStat);
	}

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
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
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
	if (isAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(currentCombo, 1, maxCombo));
		if (canNextCombo)
		{
			isComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(currentCombo == 0)
		AttackStartComboState();
		animInstance->PlayAttackMontage();
		animInstance->JumpToAttackMontageSection(currentCombo);
		isAttacking = true;
	}
}

// 애니메이션이 끝났을 때 델리게이트로 호출됨
void AABCharacter::OnAttackMontageEnded(UAnimMontage* montage, bool isInterrupted)
{
	ABCHECK(isAttacking);
	ABCHECK(currentCombo > 0);
	isAttacking = false;
	AttackEndComboState();
	
	onAttackEnd.Broadcast();
}

// 공격 시 콤보
void AABCharacter::AttackStartComboState()
{
	canNextCombo = true;
	isComboInputOn = false;
	// Clamp함수를 사용하여 min, max 사이의 값만 들어가도록 고정
	currentCombo = FMath::Clamp<int32>(currentCombo + 1, 1, maxCombo);
	//ABLOG(Warning, TEXT("%d"), currentCombo);
}

void AABCharacter::AttackEndComboState()
{
	isComboInputOn = false;
	canNextCombo = false;
	currentCombo = 0;
}

void AABCharacter::AttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);
	bool result = GetWorld()->SweepSingleByChannel(
		hitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * attackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(attackRadius),
		params);

#if ENABLE_DRAW_DEBUG

	FVector traceVec = GetActorForwardVector() * attackRange;
	FVector center = GetActorLocation() + traceVec * 0.5f;
	float halfHeight = attackRange * 0.5f + attackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = result ? FColor::Green : FColor::Red;
	float debugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		center,
		halfHeight,
		attackRadius,
		capsuleRot,
		drawColor,
		false,
		debugLifeTime
	);
	
#endif

	if (result)
	{
		if (hitResult.Actor.IsValid())
		{
			//ABLOG(Warning, TEXT("Hit Actor Name : %s"), *hitResult.Actor->GetName());
 
			FDamageEvent damageEvent;
			hitResult.Actor->TakeDamage(characterStat->GetAttack(), damageEvent, GetController(), this);

		}
	}
}

float AABCharacter::TakeDamage(float damageAmount, struct FDamageEvent const& damageEvent,
	class AController* eventInstigator, AActor* damageCauser)
{
	float finalDamage = Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);
	//ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), finalDamage);

	if (finalDamage > 0.0f)
	{
		
		characterStat->SetDamage(finalDamage);

		//FTimerHandle deadHandle;
		//GetWorldTimerManager().SetTimer(deadHandle, this, &AABCharacter::Dead, 1.0f, false);

	}

	return finalDamage;
}

void AABCharacter::Dead()
{
	Destroy(this);
}

bool AABCharacter::CanSetWeapon()
{
	return (nullptr == currentWeapon);
}

void AABCharacter::SetWeapon(class AABWeapon* newWeapon)
{
	FName weaponSocket(TEXT("hand_rSocket"));
	if (newWeapon != nullptr)
	{
		newWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, weaponSocket);
		newWeapon->SetOwner(this);
		currentWeapon = newWeapon;
	}
}

void AABCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::DIABLO);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

void AABCharacter::OnAssetLoadCompleted()
{
	assetStreamingHandle->ReleaseHandle();
	ABLOG(Warning, TEXT("OnAssetLoadCompleted"));
	TSoftObjectPtr<USkeletalMesh> loadedAssetPath(characterAssetToLoad);
	ABCHECK(loadedAssetPath.IsValid());

	GetMesh()->SetSkeletalMesh(loadedAssetPath.Get());
	SetCharacterState(ECharacterState::READY);
}