// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "ABAIController.h"
#include "ABCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	isAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
	UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(ownerComp, nodeMemory);

	auto character = Cast<AABCharacter>(ownerComp.GetAIOwner()->GetPawn());
	
	if (!character) return EBTNodeResult::Failed;

	character->Attack();
	isAttacking = true;
	character->onAttackEnd.AddLambda([this]()->void {
		isAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);
	if (!isAttacking)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}

}