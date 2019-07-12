// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolPos.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(ownerComp, nodeMemory);

	auto controllingPawn = ownerComp.GetAIOwner()->GetPawn();

	if (!controllingPawn) return EBTNodeResult::Failed;

	UNavigationSystem* navSystem = 
		UNavigationSystem::GetNavigationSystem(controllingPawn->GetWorld());
	
	if (!navSystem) return EBTNodeResult::Failed;

	FVector origin = ownerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey);
	FNavLocation nextPatrol;

	if (navSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, nextPatrol))
	{
		ownerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::PatrolPosKey, nextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}