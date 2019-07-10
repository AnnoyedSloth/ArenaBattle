// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO
	};

	void ChangeMode();
	void SetControlMode(EControlMode controlMode);

	EControlMode curControlMode = EControlMode::DIABLO;
	FVector directionToMove = FVector::ZeroVector;

	float armLengthTo = 0.0f;
	FRotator armRotationTo = FRotator::ZeroRotator;

	// adjust arm length interpolation speed
	float armLengthSpeed = 0.0f;
	// adjust arm rotation interpolation speed
	float armRotationSpeed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float damageAmount, struct FDamageEvent const& damageEvent,
		class AController* eventInstigator, AActor* damageCauser) override;


private:
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = true))
		USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = true))
		UCameraComponent* camera;

	UPROPERTY()
		class UABAnimInstance* animInstance;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		bool isAttacking;

	// Combo attack system properties
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		bool canNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		bool isComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		int32 currentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		int32 maxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		float attackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		float attackRadius;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* weapon;

private:

	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void Attack();

	// Combo attack system methods
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

	// AddDynamic 등 Delegate를 이용하기 위해선 UFUNCTION 매크로 지정이 필수!
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* montage, bool isInterrupted);

	void Dead();

};