// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Bird.generated.h"

class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Turn(float value);
	void LookUp(float value);

protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputMappingContext* BirdMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputAction* LookAction;

	void Look(const FInputActionValue& value);

	void Move(const FInputActionValue& value);

private:

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

};
