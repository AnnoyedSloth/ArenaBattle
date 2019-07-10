// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

// ��Ƽĳ��Ʈ ��������Ʈ ����
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
		float curPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
		bool isInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
		bool isDead;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		UAnimMontage* attackMontage;

	//��Ÿ��, �ִϸ��̼� �� Notifies�� ����ϸ� AnimNotify_ �ڿ� ���� �̸��� �Լ��� ȣ���Ѵ�
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 section);

	//APawn* pawn;

protected:
	void NativeUpdateAnimation(float DeltaTime) override;

public:
	UABAnimInstance();
	
	void PlayAttackMontage();

	FOnNextAttackCheckDelegate onNextAttackCheck;
	FOnAttackHitCheckDelegate onAttackHitCheck;

	void JumpToAttackMontageSection(int32 newSection);

	void SetDeadAnim() { isDead = true; }
	
};
