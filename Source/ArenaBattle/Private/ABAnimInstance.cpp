// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	curPawnSpeed = 0.0f;

	isInAir = false;
	isDead = false;

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
	if (!::IsValid(pawn)) return;
	
	if(!isDead)
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

void UABAnimInstance::JumpToAttackMontageSection(int32 newSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(newSection), attackMontage);
}

void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	//ABLOG_S(Warning)
	onAttackHitCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	//ABLOG_S(Warning)
	onNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), section));
}