// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameInstance.h"

UABGameInstance::UABGameInstance()
{
	FString characterDataPath = TEXT("/Game/GameData/ABCharacterData.ABCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_ABCHARACTER(*characterDataPath);

	if (DT_ABCHARACTER.Succeeded())
	{
		ABCharacterTable = DT_ABCHARACTER.Object;
		ABLOG(Warning, TEXT("found Character table"))
	}
	else
	{
		ABLOG(Warning, TEXT("found no Character table"))
	}
	//ABCHECK(DT_ABCHARACTER.Succeeded());
	//ABCHECK(ABCharacterTable->RowMap.Num() > 0);

}

void UABGameInstance::Init()
{
	Super::Init();
	//ABLOG(Warning, TEXT("DropExp of Level 20 ABCharacter : %d"), GetABCharacterData(20)->dropExp);
}

FABCharacterData* UABGameInstance::GetABCharacterData(int32 level)
{
	if (!ABCharacterTable)
	{
		ABLOG(Warning, TEXT("found no Character table"))
		return nullptr;
	}
	else
	{
		ABLOG(Warning, TEXT("found Character table"))
		return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(level), TEXT(""));
	}
}