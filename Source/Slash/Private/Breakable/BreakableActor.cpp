// Fill out your copyright notice in the Description page of Project Settings.

#include "Breakable/BreakableActor.h"
#include "Items/Treasure.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/CapsuleComponent.h"



ABreakableActor::ABreakableActor()
{
 	PrimaryActorTick.bCanEverTick = true;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
    SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);


}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::OnBreakActor);
}


void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::OnBreakActor(const FChaosBreakEvent&)
{

	if (bBroken) return;
	bBroken = true;

	// Set Overlap events off after breaking so the geometry collection doesnt pickup treasure
	GeometryCollection->SetGenerateOverlapEvents(false);

	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 0) {

		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		int Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);

		World->SpawnActor<ATreasure>(
			TreasureClasses[Selection],
			Location,
			GetActorRotation()
		);
	}
}

void ABreakableActor::GetHit_Implementation(const FVector& HitLocation, AActor* Hitter)
{

}
