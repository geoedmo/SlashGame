// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class UCapsuleComponent;
UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	
	UFUNCTION()	
	void OnBreakActor(const FChaosBreakEvent& BreakEvent);

	virtual void GetHit_Implementation(const FVector& HitLocation, AActor* Hitter) override;
	ABreakableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;	


	virtual void Tick(float DeltaTime) override;
protected:

	virtual void BeginPlay() override;

private:	
	bool bBroken = false;
	
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

public:
	// setters and getters
};
