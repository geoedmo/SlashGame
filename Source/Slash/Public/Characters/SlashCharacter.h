// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "Interface/PickupInterface.h"
#include "Interface/HealInterface.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class UAnimMontage;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class ASoul;
class ATreasure;
class AHealPickup;
class USlashOverlay;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickupInterface, public IHealInterface
{
	GENERATED_BODY()
public:



	ASlashCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** <IHitInterface> **/
	virtual void GetHit_Implementation(const FVector& HitLocation, AActor* Hitter) override;
	/** </IHitInterface> **/
		/** <AActor> **/
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	/** </AActor> **/

	virtual void Tick(float DeltaTime)	override;

	bool HealsNotActive();

	virtual void SetOverlappingItem(class AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Gold) override;
	virtual void ReceiveHealingPickUp(AHealPickup* Heal) override;

protected:
	/***
	* Variables
	**/
	UPROPERTY()
	USlashOverlay* SlashOverlay;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EPress;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;


	/***
	* Functions
	**/

	virtual void BeginPlay() override;
	/**Input Functions**/

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& value);
	void Turn(const FInputActionValue& value);
	void EKeyPressed();
	void MoveForward(float Value);	
	virtual void Jump() override;
	void Dodge();

	bool HasEnoughStamina();


	/** Character Operations **/
	void Arm();
	void Disarm();
	void AttachNewWeapon(); // Callback for Equip
	void SetCharacterWeaponType();
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void AttackEnd() override; // called in blueprint event, after the notify of the end of the animation
	virtual void HitReactEnd() override;
	virtual void DodgeMontageEnd() override;
private:
	/***
	* Components and Addons
	**/
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* EyeBrows;
	
	/**Animation Montages **/ 
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* EquipMontage;

	
	/***
	* Variables
	**/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState;	
	UPROPERTY(VisibleAnywhere, Category = Combat)
	EWeaponType EquippedWeaponType;

	UPROPERTY(VisibleInstanceOnly, Category = Interact)
	AItem* OverlappingItem;
	UPROPERTY(VisibleInstanceOnly, Category = Interact)
	ASword* WeaponOverlapped;

	/***
	* Functions
	**/
	virtual void Die_Implementation() override;

	bool IsUnoccupied();
	void SetHUDHealth();
	void PlayEquipMontage(FName SectionName);

	/** Event Calls **/
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	/* Equipping Functions*/
	void SetEquippedWeaponState();
	bool CanDisarm();
	bool CanArm();

public:
	FORCEINLINE UAttributeComponent* GetAttributes() const { return Attributes; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; } // Expose for getting action state to change equipping montages in BPs
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; } // getter for the character state at any moment
};


