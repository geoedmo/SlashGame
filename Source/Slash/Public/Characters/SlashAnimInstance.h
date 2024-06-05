// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "SlashAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, category = Movement)
	class ASlashCharacter* SlashCharacter;
	UPROPERTY(BlueprintReadOnly, category = Movement)
	class UNavMovementComponent* NavMovementComponent;
	class UMovementComponent* NoFallMovementComponent; // this does not contain the ability to access the isFalling() function, used UNavMovementComponent instead
	UPROPERTY(BlueprintReadOnly, category = Movement)
	float GroundSpeed;
	UPROPERTY(BlueprintReadOnly, category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, category = Movement)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, category = Movement)
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, category = Movement)
	TEnumAsByte<EDeathPose> DeathPose;

};
