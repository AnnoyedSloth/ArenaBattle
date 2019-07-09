// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerController.h"

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//ABLOG_S(Warning);
}

void AABPlayerController::Possess(APawn* pawn)
{
	//ABLOG_S(Warning);
	Super::Possess(pawn);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
}