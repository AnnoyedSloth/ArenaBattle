// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();

}

void AABGameMode::PostLogin(APlayerController* newPlayer)
{
	//ABLOG(Warning, TEXT("Postlogin begin"))
	Super::PostLogin(newPlayer);
	//ABLOG(Warning, TEXT("Postlogin end"))
}
