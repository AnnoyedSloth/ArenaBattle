// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerState.h"
#include "ABGameInstance.h"

AABPlayerState::AABPlayerState()
{
	characterLevel = 1;
	gameScore = 0;
	exp = 0;
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
	SetCharacterLevel(5);
	gameScore = 0;
}

float AABPlayerState::GetExpRatio() const
{
	if (currentStatData->nextExp <= KINDA_SMALL_NUMBER) return 0.0f;

	float result = (float)exp / (float)currentStatData->nextExp;
	ABLOG(Warning, TEXT("Ratio : %f, current : %d, Next : %d"), result, exp, currentStatData->nextExp);
	
	return result;
}

bool AABPlayerState::AddExp(int32 incomeExp)
{
	if (currentStatData->nextExp == -1) return false;

	bool didLevelUp = false;
	exp = exp + incomeExp;

	if (exp >= currentStatData->nextExp)
	{
		exp -= currentStatData->nextExp;
		SetCharacterLevel(characterLevel + 1);
		didLevelUp = true;
	}

	onPlayerStateChanged.Broadcast();
	return didLevelUp;
}

void AABPlayerState::SetCharacterLevel(int32 newCharacterLevel)
{
	auto gameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(gameInstance);

	currentStatData = gameInstance->GetABCharacterData(newCharacterLevel);
	ABCHECK(currentStatData);

	characterLevel = newCharacterLevel;
}