// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerController.h"
#include "ABHUDWidget.h"
#include "ABPlayerState.h"
#include "ABCharacter.h"

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget>
		UI_HUD(TEXT("/Game/UI/UI_HUD.UI_HUD_C"));

	if (UI_HUD.Succeeded())
	{
		hudWidgetClass = UI_HUD.Class;
	}

}

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

	hudWidget = CreateWidget<UABHUDWidget>(this, hudWidgetClass);
	hudWidget->AddToViewport();

	myPlayerState = Cast<AABPlayerState>(PlayerState);
	ABCHECK(myPlayerState);
	hudWidget->BindPlayerState(myPlayerState);
	myPlayerState->onPlayerStateChanged.Broadcast();

}

void AABPlayerController::NPCKill(AABCharacter* killedNPC) const
{
	myPlayerState->AddExp(killedNPC->GetExp());
}