// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"
/**
ENum for the Item State, which helps with hovering and non-hovering when equipped
*/
enum class EIS_ItemState {
	EIS_Hovering,
	EIS_Equipped
};

class USphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundBase;
UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnSphereOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	

protected:

	virtual void BeginPlay() override;
	/***
	* Variables
	**/
	EIS_ItemState ItemState;
	UPROPERTY(EditAnywhere, Category = "Heal Property")
	UNiagaraSystem* EffectSystem;
	/** Components **/
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, Category = Pickup)
	UNiagaraComponent* ItemEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;


	/** Sinusoudal Movements **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator Offset = { 0.f, 0.f, 0.f }; // Default no Rotation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float RunningTime;
	
	/** Sinusoudal Functions **/
	UFUNCTION(BlueprintPure)
	float TransformedSin();
	UFUNCTION(BlueprintPure)
	float TransformedCos();

	/** Effect Functions **/
	void PlayPickupSound();
	void PlayPickupEffect();

private:

	/***
	* Variables
	**/
	/** Components **/
	UPROPERTY(EditAnywhere, Category = Pickup)
	UNiagaraSystem* PickupEffect;
	UPROPERTY(EditAnywhere, Category = Pickup)
	USoundBase* PickupSound;

public:

	FORCEINLINE UNiagaraSystem* GetNiagaraSystem() { return EffectSystem; }
};

