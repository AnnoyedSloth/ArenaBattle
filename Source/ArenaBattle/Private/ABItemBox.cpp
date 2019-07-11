// Fill out your copyright notice in the Description page of Project Settings.

#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"


// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE"));

	RootComponent = trigger;
	box->SetupAttachment(RootComponent);
	effect->SetupAttachment(RootComponent);

	trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
	{
		box->SetStaticMesh(SM_BOX.Object);
	}
	box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	trigger->SetCollisionProfileName(TEXT("ItemBox"));
	box->SetCollisionProfileName(TEXT("NoCollision"));

	weaponItemClass = AABWeapon::StaticClass();

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		effect->SetTemplate(P_CHESTOPEN.Object);
		effect->bAutoActivate = false;
	}

}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, 
	bool bFromSweep, const FHitResult& sweepResult)
{
	//ABLOG_S(Warning);

	AABCharacter* player = Cast<AABCharacter>(otherActor);

	if (player && weaponItemClass)
	{
		if (player->CanSetWeapon())
		{
			AABWeapon* newWeapon = GetWorld()->SpawnActor<AABWeapon>(weaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			player->SetWeapon(newWeapon);
			effect->Activate(true);
			box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
		}
	}
}

void AABItemBox::OnEffectFinished(class UParticleSystemComponent* pSystem)
{
	Destroy();
}