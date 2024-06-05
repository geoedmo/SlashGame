// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Characters/CharacterTypes.h"
#include "Sword.generated.h"

/**
 * 
 */

class USoundBase;
class UBoxComponent;
class USceneComponent;

UCLASS()
class SLASH_API ASword : public AItem
{
	GENERATED_BODY()
	
public:
	ASword();

	void Equip(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmbers();
	void DisableSphereCollision();
	void PlayEquipSound();
	void DropWeapon();

	void AttachToSocketName(USceneComponent* InParent, const FName& InSocketName);

	TArray<AActor*> IgnoreActors;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& location);

	UFUNCTION()
	void OnBoxOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

private:


	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* SwordShink;

	UPROPERTY(EditAnywhere, Category = "Weapon Type")
	EWeaponType WeaponType = EWeaponType::EWT_OneHandWeapon;

	// Trace Variables ----------

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxStartTrace;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxEndTrace;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* WeaponBox;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowDebugBox = false;

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	void BoxTrace(FHitResult& BoxHit);

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
};
