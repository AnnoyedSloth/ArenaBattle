// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacterWidget.h"
#include "ABCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UABCharacterWidget::BindCharacterStat(UABCharacterStatComponent* newCharacterStat)
{
	ABCHECK(newCharacterStat != nullptr);

	currentCharacterStat = newCharacterStat;

	newCharacterStat->onHPChanged.AddUObject(this, &UABCharacterWidget::UpdateHPWidget);
	//newCharacterStat->onHPChanged.AddLambda([this]()->void{

	//	if (currentCharacterStat.IsValid())
	//	{
	//		ABLOG(Warning, TEXT("HPRatio : %f"), currentCharacterStat->GetHPRatio());
	//	}
	//	else ABLOG(Warning, TEXT("Current characterStat is not valid"));


	//});

}

void UABCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	hpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	ABCHECK(hpProgressBar != nullptr);
	UpdateHPWidget();
}

void UABCharacterWidget::UpdateHPWidget()
{
	if (currentCharacterStat.IsValid())
	{
		if (hpProgressBar != nullptr)
		{
			hpProgressBar->SetPercent(currentCharacterStat->GetHPRatio());
		}
	}
}