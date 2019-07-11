// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

private:
	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent* trigger;
	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* box;

	UPROPERTY(EditInstanceOnly, Category = Box)
		TSubclassOf<class AABWeapon> weaponItemClass;

	UPROPERTY(VisibleAnywhere, Category = Box)
		UParticleSystemComponent* effect;

private:
	UFUNCTION()
		void OnCharacterOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, 
			UPrimitiveComponent* otherComp, int32 otherBodyIndex, 
			bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* pSystem);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
