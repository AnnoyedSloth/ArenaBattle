// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UABCharacterStatComponent* characterState);
	void BindPlayerState(class AABPlayerState* playerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UABCharacterStatComponent> currentCharacterStat;
	TWeakObjectPtr<class AABPlayerState> currentPlayerState;

	UPROPERTY()
		class UProgressBar* hpBar;
	UPROPERTY()
		class UProgressBar* expBar;
	UPROPERTY()
		class UTextBlock* playerName;
	UPROPERTY()
		class UTextBlock* playerLevel;
	UPROPERTY()
		class UTextBlock* currentScore;
	UPROPERTY()
		class UTextBlock* highScore;
	
	
};
