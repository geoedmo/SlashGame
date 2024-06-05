// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Items/Weapons/Sword.h"
#include "Components/MeshComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthHUDComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Items/Soul.h"

AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarComponent = CreateDefaultSubobject<UHealthHUDComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensorThing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensorThing"));
	PawnSensorThing->SightRadius = 4000.f;
	PawnSensorThing->SetPeripheralVisionAngle(45.f);

}
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling) 
	{
		CheckCombatTarget();
	}
	else 
	{
		CheckPatrolTarget();
	}

}
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();

	if (IsInsideAttackRadius()) {
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius()) {
		ChaseTarget();
	}

	return DamageAmount;
}
void AEnemy::GetHit_Implementation(const FVector& HitLocation, AActor* Hitter)
{	
	Super::GetHit_Implementation(HitLocation, Hitter);

	if(!IsDead()) ShowHealthBar();

	ClearAttackTimer();
	ClearPatrolTimer();
	StopAttackMontage();

	if (IsInsideAttackRadius()) {
		if (!IsDead()) StartAttackTimer();
	}

}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensorThing) PawnSensorThing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	InitializeEnemy();
	Tags.Add("Enemy");
}

void AEnemy::Destroyed()
{
	DestroyWeapon();
}
void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}
bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}
void AEnemy::Attack()
{	
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}
void AEnemy::Die_Implementation()
{
	Super::Die_Implementation();
	EnemyState = EEnemyState::EES_Dead;
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathlifeSpan);
	//Next line is for hitting enemies during death animation

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->CanCharacterStepUpOn = ECB_No;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	SpawnSouls();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarComponent) {

		HealthBarComponent->SetHealthPercent(Attributes->GetHealthPercent());

	}
}
AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();

	if (NumPatrolTargets > 0) {
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}

	return nullptr;
}
void AEnemy::PawnSeen(APawn* SeenPawn)
{
	CombatTarget = SeenPawn;

	if (CombatTargetIsDead()) return;

	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("EngageableTarget")) &&
		IsInsideCombatRadius();

	if (bShouldChaseTarget)
	{
		ClearPatrolTimer();
		ChaseTarget();
	}
}
void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	HideHealthBar();
	SpawnDefaultWeapon();
	MoveToPatrolTarget(PatrolTarget);
}
void AEnemy::PatrolTimerFinished()
{
	MoveToPatrolTarget(PatrolTarget);
}
void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}
void AEnemy::HideHealthBar()
{
	if (HealthBarComponent) {
		HealthBarComponent->SetVisibility(false);
	}
}
void AEnemy::ShowHealthBar()
{
	if (HealthBarComponent) {
		HealthBarComponent->SetVisibility(true);
	}
}
void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}
void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToPatrolTarget(PatrolTarget);
}
void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToPatrolTarget(CombatTarget);
}
bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}
bool AEnemy::IsInPatrolRadius()
{
	return InTargetRange(PatrolTarget, PatrolRadius);
}
bool AEnemy::IsInsideCombatRadius()
{
	return InTargetRange(CombatTarget, CombatRadius);
}
bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}
bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}
bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}
bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}
bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}
bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}
void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}
void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}
void AEnemy::CheckPatrolTarget()
{
	if (EnemyState == EEnemyState::EES_Dead) return;
	if (IsInPatrolRadius()) {
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitTimeMin, PatrolWaitTimeMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}
void AEnemy::CheckCombatTarget()
{

	if (IsOutsideCombatRadius()) 
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}
void AEnemy::DestroyWeapon()
{
	if (EquippedWeapon) EquippedWeapon->Destroy();
}
void AEnemy::MoveToPatrolTarget(AActor* Target)
{
	if (EnemyController == nullptr || PatrolTarget == nullptr) { return; }
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AIAcceptanceRadius);
	EnemyController->MoveTo(MoveRequest);

}
bool AEnemy::InTargetRange(AActor* Target, double Radius)
{	
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}
void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		ASword* DefaultWeapon = World->SpawnActor<ASword>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;

	}
}

void AEnemy::SpawnSouls()
{

		UWorld* World = GetWorld();
		if (World && SoulClass && Attributes)
		{
			FVector Location = GetActorLocation();
			Location.Z += 125.f;

			ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, Location, GetActorRotation());
			
			if (SpawnedSoul) {
			SpawnedSoul->SetSouls(Attributes->GetSouls());
			}
		
		}
}