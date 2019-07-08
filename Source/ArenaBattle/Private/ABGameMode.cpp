// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameMode.h"
#include "ABPawn.h"
#include "ABPlayerController.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABPawn::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> 
		BP_PAWN(TEXT("/Game/Player/MyPlayer.MyPlayer"));
	if (BP_PAWN.Succeeded())
	{
		DefaultPawnClass = BP_PAWN.Class;
	}
}

void AABGameMode::PostLogin(APlayerController* newPlayer)
{
	//ABLOG(Warning, TEXT("Postlogin begin"))
	Super::PostLogin(newPlayer);
	//ABLOG(Warning, TEXT("Postlogin end"))
}
