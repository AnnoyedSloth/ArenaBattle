// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& ownerComp,
	uint8* nodeMemory, float deltaSeconds)
{
	Super::TickNode(ownerComp, nodeMemory, deltaSeconds);

	APawn* controllingPawn = ownerComp.GetAIOwner()->GetPawn();

	if (!controllingPawn) return;

	UWorld* world = controllingPawn->GetWorld();
	FVector center = controllingPawn->GetActorLocation();
	float detectRadius = 600.0f;

	if (!world) return;

	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams collisionQueryParam(NAME_None, false, controllingPawn);

	bool result = world->OverlapMultiByChannel(
		overlapResults,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeSphere(detectRadius),
		collisionQueryParam
	);

	if (result)
	{
		for (auto overlapResult : overlapResults)
		{
			AABCharacter* character = Cast<AABCharacter>(overlapResult.GetActor());

			if (character && character->GetController()->IsPlayerController())
			{
				ownerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, character);
				DrawDebugSphere(world, center, detectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(world, character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(world, controllingPawn->GetActorLocation(), character->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}


		}
	}
	else
	{
		ownerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, nullptr);
	}

	DrawDebugSphere(world, center, detectRadius, 16, FColor::Red, false, 0.2f);


}