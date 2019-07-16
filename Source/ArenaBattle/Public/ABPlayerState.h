// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AABPlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	float GetExpRatio() const;
	bool AddExp(int32 incomeExp);

	void InitPlayerData();

	FOnPlayerStateChangedDelegate onPlayerStateChanged;

protected:
	UPROPERTY(Transient)
		int32 gameScore;

	UPROPERTY(Transient)
		int32 characterLevel;
	
	UPROPERTY(Transient)
		int32 exp;

protected:
	void SetCharacterLevel(int32 newCharacterLevel);
	struct FABCharacterData* currentStatData;
	
};
