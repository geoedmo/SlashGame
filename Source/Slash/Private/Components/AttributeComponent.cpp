// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAttributeComponent::AddSouls(int32 NumOfSouls)
{
	Souls += NumOfSouls;
}

void UAttributeComponent::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

void UAttributeComponent::RegenHealth(float DeltaHealth)
{	
	if (GoalHealthIsLessThanMaxHealth())
	{ 
	Health = FMath::Clamp(Health + DeltaHealth, 0.f, HealthPoolLevelGoal);
	} else {
	Health = FMath::Clamp(Health + DeltaHealth, 0.f, MaxHealth);
	}
}

bool UAttributeComponent::GoalHealthIsLessThanMaxHealth()
{
	return HealthPoolLevelGoal <= MaxHealth;
}

void UAttributeComponent::ReceiveDamage(float Damage)
{	
	AdjustGoalHealthDueToDamage(Damage);

	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
}

void UAttributeComponent::AdjustGoalHealthDueToDamage(float Damage)
{
	if (CurrentlyRegeneratingHealth())
	{
		HealthPoolLevelGoal = FMath::Clamp(HealthPoolLevelGoal - Damage, 0, MaxHealth);
	}
}

bool UAttributeComponent::CurrentlyRegeneratingHealth()
{
	return HealthRegenRate != 0.f;
}

float UAttributeComponent::UseStamina(float StaminaCost)
{
	return Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

void UAttributeComponent::SetHealthPoolLevelGoal(float IncomingHeal)
{	
	IncomingHealAmount = IncomingHeal;
	if (HaveCurrentHPLevelGoal()) {
		HealthPoolLevelGoal = FMath::Clamp(HealthPoolLevelGoal + IncomingHealAmount, 0.f, MaxHealth);
	}
	else {
		HealthPoolLevelGoal = FMath::Clamp(Health + IncomingHealAmount, 0.f, MaxHealth);
	}
}

bool UAttributeComponent::HaveCurrentHPLevelGoal()
{
	return HealthPoolLevelGoal != 0.f;
}

void UAttributeComponent::ResetHealthRegen()
{
	HealthRegenRate = 0.f;
	IncomingHealAmount = 0.f;
	HealthPoolLevelGoal = 0.f;
}

bool UAttributeComponent::IsAlive()
{	
	return Health > 0.f;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



}

