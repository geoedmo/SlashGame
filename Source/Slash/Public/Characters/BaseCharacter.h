// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class ASword;
class UAnimMontage;
class UNiagaraSystem;
class USceneComponent;
class UNiagaraComponent;
class UAttributeComponent;
class UCharacterEffects;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	/** <IHitInterface> **/
	virtual void GetHit_Implementation(const FVector& HitLocation, AActor* Hitter) override;
	/** </IHitInterface> **/

protected:	

	virtual void BeginPlay() override;

	/*Components*/
	UPROPERTY(EditAnywhere, Category = "Health")
	UAttributeComponent* Attributes;	

	/***
	* Variables
	*/

	// EXPERIMENTAL
	UPROPERTY(EditAnywhere, category = "Experimental")
	UCharacterEffects* CharacterEffect;

	// WORKING
	UPROPERTY(VisibleAnywhere)
	USceneComponent* CharacterEffectDisplayPoint;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;
	UPROPERTY(VisibleAnywhere)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DodgeMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* TwoHandAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = combat)
	UAnimMontage* ReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> TwoHandMontageSections;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;
	UPROPERTY(VisibleInstanceOnly, Category = Combat)
	ASword* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = Combat)
	double WarpTargetDistance = 75.f;

	UPROPERTY(VisibleAnywhere, Category = Attributes)
	bool bIncomingHeal = false;
	/***
	* Functions Called in Child Classes
	*/

	/** Event Calls **/
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd(); // callback for anim notify of Attack End
	UFUNCTION(BlueprintCallable)
	virtual void HitReactEnd();
	UFUNCTION(BlueprintCallable)
	virtual void DodgeMontageEnd();	
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	/** Combat Operations **/
	void DirectionalHitReact(const FVector& HitLocation);
	void PlayHitSound(const FVector& HitLocation);
	void PlayEffect(UNiagaraSystem* Effect);
	void StopPlayingHealingEffect();
	void SpawnHitParticles(const FVector& HitLocation);

	UFUNCTION(BlueprintNativeEvent)
	void Die();

	void StopHealing();
	bool IsAlive();
	void DisableCapsule();
	virtual void Attack();
	bool CombatTargetIsDead();
	virtual bool CanAttack();
	virtual void HandleDamage(float DamageAmount); // Healthbar UI Handler
	virtual int32 PlayAttackMontage();
	void PlayDodgeMontage();
	virtual int32 PlayDeathMontage();
	bool ZeroMontageSections(const TArray<FName>& SectionNames);
	bool TwoHandWeaponIsValid();
	void StopAttackMontage();

	void DisableMeshCollision();
	void DisableOverlapEvents();

private:

	/***
	* Components and Addons
	*/
	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticles;

	/***
	* Variables
	*/

	/***
	* Functions 
	*/
	void PlayReactMontage(FName SectionName);
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; } // getter for the character state at any moment
};
