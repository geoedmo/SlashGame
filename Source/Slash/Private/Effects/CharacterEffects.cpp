// Fill out your copyright notice in the Description page of Project Settings.

#include "Effects/CharacterEffects.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties

UCharacterEffects::UCharacterEffects()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCharacterEffects::BeginPlay()
{
	Super::BeginPlay();

}

void UCharacterEffects::InitializeEffect(AActor* Actor, UNiagaraSystem* EffectSystem, USceneComponent* DisplayPoint)
{
	SelectedEffect = EffectSystem;
	if (SelectedEffect) {

		FVector Location = GetOwner()->GetActorLocation();
		Location.Z += 10.f;

		EffectInstance = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SelectedEffect,
			DisplayPoint,
			FName("Body Effects"),
			Location,
			Actor->GetActorRotation(),
			EAttachLocation::KeepWorldPosition,
			true
		);
	}
}

void UCharacterEffects::ActivateEffect()
{

	if (EffectInstance) {
		EffectInstance->Activate();
		UE_LOG(LogTemp, Warning, TEXT("Effect Playing"));
	}

}

void UCharacterEffects::DeactivateEffect()
{
	if (EffectInstance) {
		EffectInstance->Deactivate();
		UE_LOG(LogTemp, Warning, TEXT("Effect Stopped"));
	}

}


void UCharacterEffects::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

