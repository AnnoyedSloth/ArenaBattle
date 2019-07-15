// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "ABGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterData() : level(1), maxHP(100.0f), attack(10.0f), dropExp(10), nextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int32 level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		float maxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		float attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int32 dropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int32 nextExp;

};

UCLASS()
class ARENABATTLE_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UABGameInstance();

	virtual void Init() override;
	FABCharacterData* GetABCharacterData(int32 level);

	FStreamableManager streamableManager;
private:
	UPROPERTY()
		class UDataTable* ABCharacterTable;
	
	
};
