// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthHUDComponent;
class AAIController;
class UPawnSensingComponent;
class ASoul;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;	


	/** <AActor> **/
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	/** </AActor> **/

	/** <IHitInterface> **/
	virtual void GetHit_Implementation(const FVector& HitLocation, AActor* Hitter) override;
	/** </IHitInterface> **/

protected:
	virtual void BeginPlay() override;



	/** <AActor> **/
	virtual void Destroyed() override;
	/** </AActor> **/

	/** <BaseCharacter>	**/
	
	/***
	* Variables
	**/
	UPROPERTY(VisibleAnywhere, Category = "Health")
	UHealthHUDComponent* HealthBarComponent;
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathlifeSpan = 8.f;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	
	/***
	* Functions
	**/
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void Die_Implementation();

	virtual void HandleDamage(float DamageAmount);	
	AActor* ChoosePatrolTarget();
	/** </BaseCharacter> **/

private:
	/***
	* Components and Addons
	**/
	UPROPERTY(VisibleAnywhere, Category = "Sensing")
	UPawnSensingComponent* PawnSensorThing;
	UPROPERTY()
	AAIController* EnemyController;

	UPROPERTY(EditAnywhere, category = Combat)
	TSubclassOf<ASoul> SoulClass;

	/***
	* Variables
	**/

	// AI
	UPROPERTY(EditAnywhere, category = Combat)
	TSubclassOf<class ASword> WeaponClass;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(EditAnywhere, Category = Combat)
	double AIAcceptanceRadius = 50.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	double CombatRadius = 500.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	double PatrolRadius = 200.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	double AttackRadius = 150.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;

	// Timers
	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitTimeMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitTimeMax = 10.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	/***
	* Functions
	**/

	// Callbacks
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	void InitializeEnemy();
	/** AI Behavior **/
	void PatrolTimerFinished();
	void StartAttackTimer();	
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsInPatrolRadius();
	bool IsInsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsEngaged();
	bool IsDead();
	void ClearPatrolTimer();
	void ClearAttackTimer();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void DestroyWeapon();
	void MoveToPatrolTarget(AActor* Target);
	bool InTargetRange(AActor* Target, double Radius);
	void SpawnDefaultWeapon();
	void SpawnSouls();

};
