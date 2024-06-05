// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ASoul : public AItem
{
	GENERATED_BODY()



public:
	ASoul();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void OnSphereOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	float DesiredZ = 50.f;

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	float DriftRate = -15.f;

	FVector SoulDriftLineHit;




public:
	FORCEINLINE int32 GetSoulsCount() const { return Souls; }
	FORCEINLINE void SetSouls(int32 SetSouls) { Souls = SetSouls; }
};
