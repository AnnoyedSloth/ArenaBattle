// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerState.h"

AABPlayerState::AABPlayerState()
{
	characterLevel = 1;
	gameScore = 0;
}

int32 AABPlayerState::GetGameScore() const
{
	return gameScore;
}

int32 AABPlayerState::GetCharacterLevel() const
{
	return characterLevel;
}

void AABPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	characterLevel = 5;
	gameScore = 0;
}


