// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	
protected:
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, 
		uint8* nodeMemory, float deltaSeconds) override;

private:
	bool isAttacking = false;
	
};
