// Fill out your copyright notice in the Description page of Project Settings.

#include "ABSection.h"
#include "ABCharacter.h"
#include "ABItemBox.h"


// Sets default values
AABSection::AABSection()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = mesh;

	FString assetPath = TEXT("/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*assetPath);
	if (SM_SQUARE.Succeeded())
	{
		mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else
	{
		ABLOG(Error, TEXT("Failed to load staticmesh asset. : %s"), *assetPath);
	}

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	trigger->SetupAttachment(RootComponent);
	trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	trigger->SetCollisionProfileName(TEXT("ABTrigger"));


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnTriggerBeginOverlap);

	FString gateAssetPath = TEXT("/Game/Book/StaticMesh/SM_GATE.SM_GATE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*gateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		ABLOG(Error, TEXT("Failed to load staticMesh Asset. : %s"), *gateAssetPath);
	}

	static FName gateSockets[] = { {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	for (FName gateSocket : gateSockets)
	{
		ABCHECK(mesh->DoesSocketExist(gateSocket));
		UStaticMeshComponent* newGate = CreateDefaultSubobject<UStaticMeshComponent>(*gateSocket.ToString());
		newGate->SetStaticMesh(SM_GATE.Object);
		newGate->SetupAttachment(RootComponent, gateSocket);
		newGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		gateMeshes.Add(newGate);

		UBoxComponent* newGateTrigger = CreateDefaultSubobject<UBoxComponent>(*gateSocket.ToString().Append(TEXT("Trigger")));
		newGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		newGateTrigger->SetupAttachment(RootComponent, gateSocket);
		newGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		newGateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
		gateTriggers.Add(newGateTrigger);

		newGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnGateTriggerBeginOverlap);
		newGateTrigger->ComponentTags.Add(gateSocket);
	}

	

	noBattle = false;

	enemySpawnTime = 2.0f;
	itemBoxSpawnTime = 5.0f;

}

// Called when the game starts or when spawned
void AABSection::BeginPlay()
{
	Super::BeginPlay();

	SetState(noBattle ? ESectionState::COMPLETE : ESectionState::READY);

}

// Called every frame
void AABSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABSection::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);
	SetState(noBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void AABSection::SetState(ESectionState newState)
{
	switch (newState)
	{
	case ESectionState::READY:
		trigger->SetCollisionProfileName(TEXT("ABTrigger"));
		for (UBoxComponent* gateTrigger : gateTriggers)
		{
			gateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(true);
		//ABLOG(Warning, TEXT("New State READY"));
		break;

	case ESectionState::BATTLE:

		trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* gateTrigger : gateTriggers)
		{
			gateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(false);

		GetWorld()->GetTimerManager().SetTimer(spawnNPCTimerHandle,
			FTimerDelegate::CreateUObject(this, &AABSection::OnNPCSpawn),
			enemySpawnTime, false);

		GetWorld()->GetTimerManager().SetTimer(spawnItemBoxTimerHandle,
			FTimerDelegate::CreateLambda([this]() -> void {

			FVector2D randXY = FMath::RandPointInCircle(600.0f);
			GetWorld()->SpawnActor<AABItemBox>(GetActorLocation() + FVector(randXY, 30.0f),
				FRotator::ZeroRotator);
		}), itemBoxSpawnTime, false);

		//ABLOG(Warning, TEXT("New State BATTLE"));
		break;

	case ESectionState::COMPLETE:
		trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* gateTrigger : gateTriggers)
		{
			gateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
		}
		OperateGates(true);
		//ABLOG(Warning, TEXT("New State COMPLETE"));
		break;
	}

	currentState = newState;
}

void AABSection::OperateGates(bool isOpen)
{
	for (UStaticMeshComponent* gate : gateMeshes)
	{
		gate->SetRelativeRotation(isOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

void AABSection::OnTriggerBeginOverlap(
	UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep,
	const FHitResult &sweepResult)
{
	if (currentState == ESectionState::READY)
	{
		ABLOG_S(Warning);
		SetState(ESectionState::BATTLE);
	}
}

void AABSection::OnGateTriggerBeginOverlap(
	UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex,
	bool fromSweep, const FHitResult& sweepResult)
{
	//ABCHECK(overlappedComponent->ComponentTags.Num() == 1);

	ABLOG(Warning, TEXT("OnGateTriggerBeginOverlap"));

	FName componentTag = overlappedComponent->ComponentTags[0];
	FName socketName = FName(*componentTag.ToString().Left(2));

	if (!mesh->DoesSocketExist(socketName)) return;

	FVector newLocation = mesh->GetSocketLocation(socketName);

	TArray<FOverlapResult> overlapResults;
	
	FCollisionQueryParams collisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams objectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
	bool result = GetWorld()->OverlapMultiByObjectType(
		overlapResults,
		newLocation,
		FQuat::Identity,
		objectQueryParam,
		FCollisionShape::MakeSphere(775.0f),
		collisionQueryParam
	);

	if (!result)
	{
		auto newSection = GetWorld()->SpawnActor<AABSection>(newLocation, FRotator::ZeroRotator);
	}
	else
	{
		ABLOG(Warning, TEXT("New section area is not empty."));
	}

}

void AABSection::OnNPCSpawn()
{
	GetWorld()->SpawnActor<AABCharacter>(GetActorLocation() + FVector::UpVector * 88.0f,
		FRotator::ZeroRotator);
}