// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacterStatComponent.h"
#include "ABGameInstance.h"


// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	level = 1;

	// ...
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(level);
}


// Called every frame
void UABCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UABCharacterStatComponent::SetNewLevel(int32 newLevel)
{
	UABGameInstance* gameInstance = 
		Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if(gameInstance) currentStatData = gameInstance->GetABCharacterData(newLevel);
	
	if (currentStatData != nullptr)
	{
		level = newLevel;
		SetHP(currentStatData->maxHP);
	}
	else
	{
		ABLOG(Error, TEXT("Level (%d) data doesn't exist"), newLevel);
	}
}

void UABCharacterStatComponent::SetDamage(float newDamage)
{
	ABCHECK(currentStatData != nullptr);

	SetHP(FMath::Clamp<float>(currentHP - newDamage, 0.0f, currentStatData->maxHP));
}

float UABCharacterStatComponent::GetAttack()
{
	return currentStatData->attack;
}

void UABCharacterStatComponent::SetHP(float newHP)
{
	currentHP = newHP;
	onHPChanged.Broadcast();
	if (currentHP < KINDA_SMALL_NUMBER)
	{
		currentHP = 0.0f;
		onHPIsZero.Broadcast();
	}
}

float UABCharacterStatComponent::GetHPRatio()
{
	return (currentStatData->maxHP < KINDA_SMALL_NUMBER) ? 0.0f : (currentHP / currentStatData->maxHP);
}

int32 UABCharacterStatComponent::GetDropExp() const
{
	return currentStatData->dropExp;
}