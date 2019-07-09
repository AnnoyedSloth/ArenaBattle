// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	curPawnSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/Animation/Montage/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		attackMontage = ATTACK_MONTAGE.Object;
	}

	//pawn = TryGetPawnOwner();
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	auto pawn = TryGetPawnOwner();
	if (::IsValid(pawn))
	{
		curPawnSpeed = pawn->GetVelocity().Size();
		auto character = Cast<ACharacter>(pawn);
		if (character)
		{
			isInAir = character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(attackMontage))
	{
		Montage_Play(attackMontage, 1.0f);
	}
}

void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	ABLOG_S(Warning)
}