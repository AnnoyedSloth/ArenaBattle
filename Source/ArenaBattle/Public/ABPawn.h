// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "ABPawn.generated.h"

UCLASS()
class ARENABATTLE_API AABPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AABPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PostInitializeComponents() override;
	void PossessedBy(AController* newController);

	void UpDown(float value);
	void LeftRight(float value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
		UCapsuleComponent* capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
		UFloatingPawnMovement* movement;
	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
		USpringArmComponent* springArm;
	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
		UCameraComponent* camera;
	
};
