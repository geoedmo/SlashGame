// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Interface/HealInterface.h"
#include "HealPickup.generated.h"

/**
 * 
 */
class UNiagaraSystem;
class UAttributeComponent;
class USlashOverlay;

UCLASS()
class SLASH_API AHealPickup : public AItem, public IHealInterface
{
	GENERATED_BODY()

public:
	AHealPickup();

	virtual void Tick(float DeltaTime) override;

	void ApplyHealing(AActor* OtherActor, UAttributeComponent* Attributes);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Heal Property");
	bool bItemHealingEnabled;
	UPROPERTY(VisibleAnywhere, Category = "Heal Property");
	UAttributeComponent* ActorAttributes;
	UPROPERTY(VisibleAnywhere);
	AActor* HealedActor;


	virtual void BeginPlay() override;

	virtual void OnSphereOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = Pickup)
	float HealAmount = 25.f;

	UPROPERTY(EditAnywhere, Category = Pickup)
	float HealRegenRate = 4.f;

public:
	FORCEINLINE float GetHealRegenRate() const { return HealRegenRate; }
	FORCEINLINE float GetHealAmount() const { return HealAmount; }
};
