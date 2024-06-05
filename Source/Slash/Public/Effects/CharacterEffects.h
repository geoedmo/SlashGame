// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterEffects.generated.h"

UENUM()
enum class EEffectType : uint8
{

	EET_HealOverTime	 UMETA(DisplayName = "Heal Over Time"),
	EET_Soul			 UMETA(DisplayName = "Soul")

};

UENUM()
enum class EEffectState : uint8 {

	EES_Off,
	EES_On

};

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UCharacterEffects : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UCharacterEffects();

	void InitializeEffect(AActor* Actor, UNiagaraSystem* EffectSystem, USceneComponent* DisplayPoint);
	void ActivateEffect();
	void DeactivateEffect();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UNiagaraSystem* SelectedEffect;
	UPROPERTY(EditAnywhere, Category = Effect)
	UNiagaraComponent* EffectInstance;
	UPROPERTY(EditAnywhere, Category = Effect)
	TArray<UNiagaraSystem*> Effect;

	UPROPERTY(VisibleAnywhere, category = Effect)
	EEffectType EffectType = EEffectType::EET_HealOverTime;

	EEffectState EffectState = EEffectState::EES_Off;

public:	
	FORCEINLINE EEffectState CheckEffectState() { return EffectState; }
	FORCEINLINE void SetEffectState(EEffectState State) { EffectState = State; }
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
