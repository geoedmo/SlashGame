// Fill out your copyright notice in the Description page of Project Settings.

#include "Interface/PickupInterface.h"
#include "Components/AttributeComponent.h"
#include "HUD/SlashOverlay.h"
#include "Components/SphereComponent.h"
#include "Items/HealPickup.h"
#include "NiagaraComponent.h"



AHealPickup::AHealPickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHealPickup::Tick(float DeltaTime)
{
	// Access Overlay / UI Components

	if (ActorAttributes) {

		if (ActorAttributes->GetActiveIncomingHealing() == true) {
			if (ActorAttributes->GetHealth() == 0) return;

			bool bRollHoTs =
			
				ActorAttributes->GetHealth() <
				ActorAttributes->GetHealthPoolLevelGoal();
			
			if (bRollHoTs) {
			
				ActorAttributes->RegenHealth(DeltaTime * HealRegenRate);
			
			}
			else {
				
				ActorAttributes->ResetHealthRegen();
				ActorAttributes->SetActiveIncomingHealing(false);
				
				Destroy();
				
				//CharacterEffect->SetEffectState(EEffectState::EES_Off);
				//StopPlayingHealingEffect();
			}
		}
		
	}

}

void AHealPickup::ApplyHealing(AActor* OtherActor, UAttributeComponent* Attributes)
{	
	ActorAttributes = Attributes;

	if (Attributes) {
		//Attributes->SetHealthRegenRate(HealRegenRate);
		Attributes->SetHealthPoolLevelGoal(HealAmount);
		Attributes->SetActiveIncomingHealing(true);
		//PlayHealingEffect();
	}
}

void AHealPickup::BeginPlay()
{
	Super::BeginPlay();

}

void AHealPickup::OnSphereOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface) {
		PlayPickupSound();
		//PlayPickupEffect();
	}

	IHealInterface* HealInterface = Cast<IHealInterface>(OtherActor);

	if (HealInterface) {
		HealInterface->ReceiveHealingPickUp(this);
		ItemEffect->SetVisibility(false);
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


}
