// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AABAIController();
	virtual void Possess(APawn* pawn) override;
	virtual void UnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
private:



	UPROPERTY()
		class UBehaviorTree* btAsset;
	UPROPERTY()
		class UBlackboardData* bbAsset;
	
};