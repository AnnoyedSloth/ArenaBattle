// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ArenaBattleGameMode.h"
#include "ArenaBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArenaBattleGameMode::AArenaBattleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
