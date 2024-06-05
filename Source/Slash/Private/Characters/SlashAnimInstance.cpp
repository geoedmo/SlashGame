// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/NavMovementComponent.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());

	if (SlashCharacter) {
		NavMovementComponent = SlashCharacter->GetCharacterMovement();
	}


}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (NavMovementComponent) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(NavMovementComponent->Velocity);
		IsFalling = NavMovementComponent->IsFalling(); // is falling boolean return
		CharacterState = SlashCharacter->GetCharacterState();
		ActionState = SlashCharacter->GetActionState();
		DeathPose = SlashCharacter->GetDeathPose();

	}
	
}
