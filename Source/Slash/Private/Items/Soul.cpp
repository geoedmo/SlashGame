// Fill out your copyright notice in the Description page of Project Settings.

#include "Interface/PickupInterface.h"
#include "Items/Soul.h"
#include "Kismet/KismetSystemLibrary.h"

ASoul::ASoul() {

	PrimaryActorTick.bCanEverTick = true;

}

void ASoul::BeginPlay() {

	Super::BeginPlay();

	FVector LineTraceEnd = GetActorLocation();
	LineTraceEnd.Z += -2000.f;

	const FVector Start = GetActorLocation();


	ETraceTypeQuery QueryType = ETraceTypeQuery::TraceTypeQuery1;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult LineHit;

	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		LineTraceEnd,
		QueryType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		LineHit,
		true
	);

	DesiredZ = LineHit.ImpactPoint.Z + DesiredZ;
}

void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float LocationZ = GetActorLocation().Z;

	if (LocationZ > DesiredZ) {
		FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		AddActorWorldOffset(DeltaLocation);
	}


}

void ASoul::OnSphereOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface) {
		PickupInterface->AddSouls(this);
	PlayPickupSound();
	PlayPickupEffect();
	Destroy();
	
	}


}