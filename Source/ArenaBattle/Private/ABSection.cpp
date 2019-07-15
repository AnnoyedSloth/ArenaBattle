// Fill out your copyright notice in the Description page of Project Settings.

#include "ABSection.h"


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
	}


}

// Called when the game starts or when spawned
void AABSection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

