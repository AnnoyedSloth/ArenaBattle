// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

UCLASS()
class ARENABATTLE_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* body;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* water;

	UPROPERTY(VisibleAnywhere)
		UPointLightComponent* light;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* splash;

	UPROPERTY(VisibleAnywhere)
		URotatingMovementComponent* movement;

	UPROPERTY(EditAnywhere, Category = "Status", Meta = (AllowPrivateAccess = true))
		float rotateSpeed;
	
};
