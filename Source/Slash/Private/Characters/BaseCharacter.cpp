// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "Items/Weapons/Sword.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Effects/CharacterEffects.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	CharacterEffectDisplayPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Body Effects"));
	CharacterEffectDisplayPoint->SetupAttachment(GetRootComponent());

	CharacterEffect = CreateDefaultSubobject<UCharacterEffects>(TEXT("Character Effect"));

}
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
void ABaseCharacter::GetHit_Implementation(const FVector& HitLocation, AActor* Hitter)
{

	if (IsAlive()) {
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else Die();

	PlayHitSound(HitLocation);
	SpawnHitParticles(HitLocation);
}
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}
void ABaseCharacter::AttackEnd()
{
}
void ABaseCharacter::HitReactEnd()
{
	
}
FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr) return FVector(); 
		const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
		const FVector Location = GetActorLocation();

		FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();

		TargetToMe *= WarpTargetDistance;

		return CombatTargetLocation + TargetToMe;
	return FVector();
}
FVector ABaseCharacter::GetRotationWarpTarget()
{	
	if (CombatTarget) {
	return CombatTarget->GetTargetLocation();
	}
	return FVector();
}
void ABaseCharacter::DirectionalHitReact(const FVector& HitLocation)
{
	const FVector Forward = GetActorForwardVector();
	// Flatten the Vector to the Actor Location in the Z coordinate
	const FVector ImpactLowered(HitLocation.X, HitLocation.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Foward||ToHit| * cos(theta)
	// |Foward| =1, |ToHit| =1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse Cosine (arc-cosine) of cos(theta) to get theta (angle)
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// If CrossProduct Points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0) { Theta *= -1.f; }

	FName SectionName;
	if (Theta >= -45.f && Theta < 45.f) SectionName = "FromFront";
	else if (Theta >= -135.f && Theta < -45.f) SectionName = "FromLeft";
	else if (Theta >= 45.f && Theta < 135.f) SectionName = "FromRight";
	else SectionName = "FromBack";

	PlayReactMontage(SectionName);

}
void ABaseCharacter::PlayHitSound(const FVector& HitLocation)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			HitLocation
		);
	}
}
void ABaseCharacter::PlayEffect(UNiagaraSystem* Effect)
{
	if (CharacterEffect->CheckEffectState() == EEffectState::EES_On) { return; }

	CharacterEffect->InitializeEffect(this, Effect, CharacterEffectDisplayPoint);
	CharacterEffect->SetEffectState(EEffectState::EES_On);

}
void ABaseCharacter::StopPlayingHealingEffect()
{
	if (CharacterEffect->CheckEffectState() == EEffectState::EES_Off) {

		CharacterEffect->DeactivateEffect();

	}

}

void ABaseCharacter::SpawnHitParticles(const FVector& HitLocation)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, HitLocation);
	}
}

void ABaseCharacter::Die_Implementation()
{
	StopHealing();
	PlayDeathMontage();
	Tags.Add(FName("Dead"));

}
void ABaseCharacter::StopHealing()
{
	bIncomingHeal = false;
	Attributes->ResetHealthRegen();
}
bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}
void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void ABaseCharacter::Attack()
{
	if (CombatTargetIsDead()) {
		CombatTarget = nullptr;
	}
}
bool ABaseCharacter::CombatTargetIsDead()
{
	return CombatTarget && CombatTarget->ActorHasTag(FName("Dead"));
}
bool ABaseCharacter::CanAttack()
{
	return false;
}
void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes) {
		Attributes->ReceiveDamage(DamageAmount);
	}
}
int32 ABaseCharacter::PlayAttackMontage()
{	

	if (TwoHandWeaponIsValid())
	{	
		return PlayRandomMontageSection(TwoHandAttackMontage, TwoHandMontageSections);
	}

	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

void ABaseCharacter::PlayDodgeMontage()
{	
	if (DodgeMontage) {
	PlayMontageSection(DodgeMontage, FName("Default"));
	}
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX) {
		DeathPose = Pose;
	}

	return Selection;
}
bool ABaseCharacter::ZeroMontageSections(const TArray<FName>& SectionNames)
{
	return SectionNames.Num() <= 0;
}
bool ABaseCharacter::TwoHandWeaponIsValid()
{
	return CharacterState == ECharacterState::ECS_EquippedTwohandedWeapon &&
		 !ZeroMontageSections(TwoHandMontageSections);
}
void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}
void ABaseCharacter::DodgeMontageEnd()
{

}
void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void ABaseCharacter::DisableOverlapEvents()
{
	GetMesh()->SetGenerateOverlapEvents(false);
}
void ABaseCharacter::PlayReactMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && ReactMontage) {
		AnimInstance->Montage_Play(ReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, ReactMontage);
	}
}
int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}
void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage) 
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}