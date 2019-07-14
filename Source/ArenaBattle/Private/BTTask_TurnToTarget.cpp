// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(ownerComp, nodeMemory);

	auto character = Cast<AABCharacter>(ownerComp.GetAIOwner()->GetPawn());
	if (!character) return EBTNodeResult::Failed;

	auto target = Cast<AABCharacter>(ownerComp.GetBlackboardComponent()->GetValueAsObject(AABAIController::TargetKey));

	if (!target) return EBTNodeResult::Failed;

	FVector lookVector = target->GetActorLocation() - character->GetActorLocation();
	lookVector.Z = 0.0f;
	FRotator targetRot = FRotationMatrix::MakeFromX(lookVector).Rotator();
	character->SetActorRotation(FMath::RInterpTo(character->GetActorRotation(), targetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}

