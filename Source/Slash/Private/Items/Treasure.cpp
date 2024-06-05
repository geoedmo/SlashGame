// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Interface/PickupInterface.h"

void ATreasure::OnSphereOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface) {
		UE_LOG(LogTemp, Warning, TEXT("Pickup Treasure"));
		PickupInterface->AddGold(this);
	PlayPickupSound();
	Destroy();
	}


}