// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABSection.generated.h"

UCLASS()
class ARENABATTLE_API AABSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABSection();

	virtual void OnConstruction(const FTransform& transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	enum class ESectionState : uint8
	{
		READY = 0,
		BATTLE,
		COMPLETE
	};

	void SetState(ESectionState newState);
	ESectionState currentState = ESectionState::READY;
	void OperateGates(bool isOpen = true);

	UFUNCTION()
		void OnTriggerBeginOverlap(
			UPrimitiveComponent* overlappedComponent, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep,
			const FHitResult &sweepResult);

	UFUNCTION()
		void OnGateTriggerBeginOverlap(
			UPrimitiveComponent* overlappedComponent, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodyIndex,
			bool fromSweep, const FHitResult& sweepResult);

	void OnNPCSpawn();

private:

	UPROPERTY(VisibleAnywhere, Category = Mesh, meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, Category = Mesh, meta = (AllowPrivateAccess = true))
		TArray<UStaticMeshComponent*> gateMeshes;

	UPROPERTY(VisibleAnywhere, Category = Trigger, meta = (AllowPrivateAccess = true))
		TArray<UBoxComponent*> gateTriggers;

	UPROPERTY(VisibleAnywhere, Category = Trigger, meta = (AllowPrivateAccess = true))
		UBoxComponent* trigger;
	
	UPROPERTY(EditAnywhere, Category = State, meta = (AllowPrivateAccess = true))
		bool noBattle;

	UPROPERTY(EditAnywhere, Category = Spawn, meta = (AllowPrivateAccess = true))
		float enemySpawnTime;
	
	UPROPERTY(EditAnywhere, Category = Spawn, meta = (AllowPrivateAccess = true))
		float itemBoxSpawnTime;

	FTimerHandle spawnNPCTimerHandle = {};
	FTimerHandle spawnItemBoxTimerHandle = {};
	
};