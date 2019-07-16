// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"
#include "ABPlayerState.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	PlayerStateClass = AABPlayerState::StaticClass();
}

void AABGameMode::PostLogin(APlayerController* newPlayer)
{
	//ABLOG(Warning, TEXT("Postlogin begin"))
	Super::PostLogin(newPlayer);
	//ABLOG(Warning, TEXT("Postlogin end"))

	auto playerState = Cast<AABPlayerState>(newPlayer->PlayerState);
	ABCHECK(playerState);
	playerState->InitPlayerData();
}

