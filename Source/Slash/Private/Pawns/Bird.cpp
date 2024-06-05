// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"



// Sets default values
ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(Capsule);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	
	}
	
}

void ABird::MoveForward(float Value)
{	
	if ((Controller != nullptr) && (Value != 0.f))
	{
		FVector Forward = GetActorForwardVector();
		
		AddMovementInput(Forward, Value);
	}
	UE_LOG(LogTemp, Warning, TEXT("Value: %f"), Value);
}

void ABird::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();

	if (GetController()) {

		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);

	}
}

void ABird::Move(const FInputActionValue& value)
{
	const float DirectionalValue = value.Get<float>();

	if ((Controller != nullptr) && (DirectionalValue != 0.f)) // Check for Controller pointer, and that Input from FAction is not zero
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionalValue);
	}

}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input2
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABird::MoveForward);
	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABird::Turn);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABird::LookUp);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);

	}

	
}

void ABird::Turn(float value)
{
	AddControllerYawInput(value);
}

void ABird::LookUp(float value)
{
	AddControllerPitchInput(value);
}

