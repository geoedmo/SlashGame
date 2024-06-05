// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/SlashCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Sword.h"
#include "Animation/AnimMontage.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Soul.h"
#include "Items/HealPickup.h"
#include "Effects/CharacterEffects.h"
#include "Items/Treasure.h"


/** Public **/
ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	EyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	EyeBrows->SetupAttachment(GetMesh());
	EyeBrows->AttachmentName = FString("head");
}
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);


	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Turn);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EPress, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
	}
}
void ASlashCharacter::GetHit_Implementation(const FVector& HitLocation, AActor* Hitter)
{
	Super::GetHit_Implementation(HitLocation, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReacting;
	}
}
float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);

	if (HealsNotActive())
	{
		SetHUDHealth();
	}
	return DamageAmount;

}
void ASlashCharacter::Tick(float DeltaTime)
{
	if (Attributes && SlashOverlay) {

		Attributes->RegenStamina(DeltaTime);
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());

		if (Attributes->GetActiveIncomingHealing()) {
			SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		}

		if (CharacterEffect->CheckEffectState() != EEffectState::EES_Off && HealsNotActive()) {
			CharacterEffect->SetEffectState(EEffectState::EES_Off);
			StopPlayingHealingEffect();
		}
	}
}
bool ASlashCharacter::HealsNotActive()
{
	return !Attributes->GetActiveIncomingHealing();
}
void ASlashCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}
void ASlashCharacter::AddSouls(ASoul* Soul)
{
	UE_LOG(LogTemp, Warning, TEXT("SlashCharacter Adds Souls"));
	if (Attributes && SlashOverlay) {
		Attributes->AddSouls(Soul->GetSoulsCount());
		SlashOverlay->SetSouls(Attributes->GetSouls());

		CharacterEffect->InitializeEffect(this, Soul->GetNiagaraSystem(), CharacterEffectDisplayPoint);
	}
}
void ASlashCharacter::AddGold(ATreasure* Treasure)
{
	UE_LOG(LogTemp, Warning, TEXT("SlashCharacter Adds Gold"));
	if (Attributes && SlashOverlay) {
		Attributes->AddGold(Treasure->GetGoldCount());
		SlashOverlay->SetGold(Attributes->GetGold());

	}
}
void ASlashCharacter::ReceiveHealingPickUp(AHealPickup* Heal)
{
	UE_LOG(LogTemp, Warning, TEXT("Receieved Heal Pickup Information"));
	Heal->ApplyHealing(this, Attributes);
	PlayEffect(Heal->GetNiagaraSystem());
}

/** Protected **/
void ASlashCharacter::BeginPlay()
{

	// TODO: REFACTOR THIS FUNCTION SPECIFICALLY THE CALLS TO THE HUD/OVERLAY
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}

	}
	// Add a Tag for Slash Character so we can check it for comparison in Enemy vision vs another enemy.
	Tags.Add(FName("EngageableTarget"));
	if (PlayerController) {
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());

		if (SlashHUD) {
			SlashOverlay = Cast<USlashOverlay>(SlashHUD->GetSlashOverlay());

			if (SlashOverlay) {
				SlashOverlay->SetHealthBarPercent(1.f);
				SlashOverlay->SetStaminaBarPercent(1.f);
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSouls(0);
			}
		}
	}
}
void ASlashCharacter::Move(const FInputActionValue& Value)
{
	// Hop out of move func if we are attacking
	if (ActionState != EActionState::EAS_Unoccupied) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();
	//find out which way is forward
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, MovementVector.Y);
	
	//find out which way is right
	const FVector DirectionRight = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(DirectionRight, MovementVector.X);
}
void ASlashCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}
void ASlashCharacter::Turn(const FInputActionValue& value)
{
	const float TurnValue = value.Get<float>();

	if (TurnValue != 0.f) {
		AddControllerYawInput(TurnValue);
	}
}
void ASlashCharacter::EKeyPressed()
{
	WeaponOverlapped = Cast<ASword>(OverlappingItem);
	
	if (WeaponOverlapped && EquippedWeapon) {
		EquippedWeapon->DropWeapon();
		AttachNewWeapon();
	}
	else if (WeaponOverlapped && !EquippedWeapon) {
		AttachNewWeapon();
	}
	else {
		if (CanDisarm()) 
		{
			Disarm();
		}
		else if (CanArm()) 
		{
			Arm();
		}
	}
}
void ASlashCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f)) // Check for Controller pointer, and that Input is not zero
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);
	}
}
void ASlashCharacter::Jump()
{

	if (IsUnoccupied()) {
		Super::Jump();
	} 

}

void ASlashCharacter::Dodge()
{
	if (!IsUnoccupied() || !HasEnoughStamina()) return;
	
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
		
	if (Attributes && SlashOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());

	}
}

bool ASlashCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage("Arm");
	ActionState = EActionState::EAS_Equipping;
	SetEquippedWeaponState();
}
void ASlashCharacter::Disarm()
{
	ActionState = EActionState::EAS_Equipping;
	PlayEquipMontage("Disarm");
	CharacterState = ECharacterState::ECS_Unequipped;
}
void ASlashCharacter::AttachNewWeapon()
{	
	SetCharacterWeaponType();

	WeaponOverlapped->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	EquippedWeapon = WeaponOverlapped;
	EquippedWeaponType = EquippedWeapon->GetWeaponType();

	OverlappingItem = nullptr;
	WeaponOverlapped = nullptr;
}
void ASlashCharacter::SetCharacterWeaponType()
{	
	EWeaponType WeaponType = WeaponOverlapped->GetWeaponType();
	if (WeaponType == EWeaponType::EWT_OneHandWeapon)
		CharacterState = ECharacterState::ECS_EquippedOnehandedWeapon;
	else {
		CharacterState = ECharacterState::ECS_EquippedTwohandedWeapon;
	}
}
bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}
void ASlashCharacter::Attack()
{	

	if (CanAttack()) {
	PlayAttackMontage();
	ActionState = EActionState::EAS_Attacking;
	}
	
}
void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void ASlashCharacter::HitReactEnd()
{
	Super::HitReactEnd();
	ActionState = EActionState::EAS_Unoccupied;
}
void ASlashCharacter::DodgeMontageEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void ASlashCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	CharacterEffect->SetEffectState(EEffectState::EES_Off);
	StopPlayingHealingEffect();

	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
	DisableOverlapEvents();
}
/** Private **/
bool ASlashCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}
void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}
void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage) 
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}

}
void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachToSocketName(GetMesh(), FName("SpineSocket"));
	}
}
void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachToSocketName(GetMesh(), FName("RightHandSocket"));
	}
}
void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void ASlashCharacter::SetEquippedWeaponState()
{	
	if (EquippedWeaponType == EWeaponType::EWT_OneHandWeapon)
		CharacterState = ECharacterState::ECS_EquippedOnehandedWeapon;
	else {
		CharacterState = ECharacterState::ECS_EquippedTwohandedWeapon;
	}
}
bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}
bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}