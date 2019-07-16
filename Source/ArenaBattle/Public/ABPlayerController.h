// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AABPlayerController();
	void PostInitializeComponents() override;
	void Possess(APawn* pawn) override;

	class UABHUDWidget* GetHUDWidget() const { return hudWidget; }
	
protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABHUDWidget> hudWidgetClass;

private:
	UPROPERTY()
		class UABHUDWidget* hudWidget;
	
};
