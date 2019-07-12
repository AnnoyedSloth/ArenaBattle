// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInAttackRange.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(
	UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(ownerComp, nodeMemory);

	auto controllingPawn = ownerComp.GetAIOwner()->GetPawn();
	if (!controllingPawn) return false;

	auto target = Cast<AABCharacter>(
		ownerComp.GetBlackboardComponent()->GetValueAsObject(AABAIController::TargetKey));

	if (!target) return false;

	result = (target->GetDistanceTo(controllingPawn) <= 200.0f);
	return result;
}



