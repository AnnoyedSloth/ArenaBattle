// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

// 멀티캐스트 델리게이트 선언
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

	//몽타주, 애니메이션 등 Notifies를 등록하면 AnimNotify_ 뒤에 붙은 이름의 함수를 호출한다
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
