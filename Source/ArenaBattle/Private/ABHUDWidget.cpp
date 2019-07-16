// Fill out your copyright notice in the Description page of Project Settings.

#include "ABHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ABCharacterStatComponent.h"
#include "ABPlayerState.h"

void UABHUDWidget::BindCharacterStat(UABCharacterStatComponent* characterStat)
{
	ABCHECK(characterStat);
	currentCharacterStat = characterStat;
	characterStat->onHPChanged.AddUObject(this, &UABHUDWidget::UpdateCharacterStat);
}

void UABHUDWidget::BindPlayerState(AABPlayerState* playerState)
{
	ABCHECK(playerState);
	currentPlayerState = playerState;
	playerState->onPlayerStateChanged.AddUObject(this, &UABHUDWidget::UpdatePlayerState);
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	hpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	ABCHECK(hpBar);

	expBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	ABCHECK(expBar);

	playerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	ABCHECK(playerName);

	playerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	ABCHECK(playerLevel);

	currentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	ABCHECK(currentScore);

	highScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	ABCHECK(highScore);
}

void UABHUDWidget::UpdateCharacterStat()
{
	ABCHECK(currentCharacterStat.IsValid());
	hpBar->SetPercent(currentCharacterStat->GetHPRatio());
}

void UABHUDWidget::UpdatePlayerState()
{
	ABCHECK(currentPlayerState.IsValid());
	expBar->SetPercent(currentPlayerState->GetExpRatio());
	playerName->SetText(FText::FromString(currentPlayerState->GetPlayerName()));
	playerLevel->SetText(FText::FromString(FString::FromInt(currentPlayerState->GetCharacterLevel())));
	currentScore->SetText(FText::FromString(FString::FromInt(currentPlayerState->GetGameScore())));
}