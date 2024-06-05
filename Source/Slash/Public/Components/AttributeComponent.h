// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RegenStamina(float DeltaTime);
	void RegenHealth(float DeltaHealth);

	bool GoalHealthIsLessThanMaxHealth();

protected:
	virtual void BeginPlay() override;

private:

	// Characters Health that Changes
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health = 100.f;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina = 100.f;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina = 100.f;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 14.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 3.f;

	/** Health Regen **/
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float HealthRegenRate = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Actor Atributes")
	float IncomingHealAmount = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Actor Attributes")
	float HealthPoolLevelGoal = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Actor Attributes")
	bool bActiveHealing = false;

private:
	void AdjustGoalHealthDueToDamage(float Damage);
	bool CurrentlyRegeneratingHealth();
	bool HaveCurrentHPLevelGoal();
public:

	void AddSouls(int32 NumOfSouls);
	void AddGold(int32 AmountOfGold);
	void ReceiveDamage(float Damage);
	float UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	void ResetHealthRegen();
	bool IsAlive();

	void SetHealthPoolLevelGoal(float IncomingHeal);

	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetHealthPoolLevelGoal() const { return HealthPoolLevelGoal; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE void SetHealthRegenRate(float RegenRate) { HealthRegenRate = RegenRate; };
	FORCEINLINE void SetActiveIncomingHealing(bool ActiveHealing) { bActiveHealing = ActiveHealing; }
	FORCEINLINE bool GetActiveIncomingHealing() { return bActiveHealing; }

};