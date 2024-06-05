// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Interface/PickupInterface.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/Gameplaystatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AItem::AItem()
{	
	PrimaryActorTick.bCanEverTick = true;

	//New Static Mesh Component, default Subobject
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(ItemMesh);

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ItemEffect"));
	ItemEffect->SetupAttachment(ItemMesh);

}

void AItem::BeginPlay()
{	

	Super::BeginPlay();	

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlapEvent);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereOverlapEnd);

}

float AItem::TransformedSin()
{

	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{

	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::PlayPickupSound()
{

		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this, 
				PickupSound, 
				GetActorLocation()
			);
		}

}

void AItem::PlayPickupEffect()
{
	if (PickupEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}

void AItem::OnSphereOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		PickupInterface->SetOverlappingItem(this);
	}
}

void AItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;


	if (ItemState == EIS_ItemState::EIS_Hovering)
	{
		if (Offset != FRotator::ZeroRotator) // add rotation to the item, zero is default so we need to check if we should go in here.
		{
			AddActorWorldRotation(Offset * DeltaTime);
		}

		FVector Periodic = { 0.f, 0.f, TransformedSin() };
		AddActorWorldOffset(Periodic);
	}

	

}


