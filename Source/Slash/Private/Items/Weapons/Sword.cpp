// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Sword.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Items/Item.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/HitInterface.h"
#include "NiagaraComponent.h"


ASword::ASword()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	BoxStartTrace = CreateDefaultSubobject<USceneComponent>(TEXT("Start Box Trace"));
	BoxEndTrace = CreateDefaultSubobject<USceneComponent>(TEXT("End Box Trace"));
	BoxStartTrace->SetupAttachment(GetRootComponent());
	BoxEndTrace->SetupAttachment(GetRootComponent());

	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnBoxOverlapEvent);

}

void ASword::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EIS_ItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachToSocketName(InParent, InSocketName);
	DisableSphereCollision();
	PlayEquipSound();
	DeactivateEmbers();
}

void ASword::DeactivateEmbers()
{
	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

void ASword::DisableSphereCollision()
{
	if (Sphere) {

		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASword::PlayEquipSound()
{
	if (SwordShink)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SwordShink,
			GetActorLocation()
		);
	}
}

void ASword::DropWeapon() {

	// Set Collision and Niagara because we're dropping weapon back on ground
	if (Sphere) {
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	if (ItemEffect)
	{
		ItemEffect->Activate();
	}
	// Detatch from the character
	FDetachmentTransformRules DetachWeapon(FDetachmentTransformRules::KeepWorldTransform);
	ItemMesh->DetachFromComponent(DetachWeapon);
	ItemState = EIS_ItemState::EIS_Hovering;
	ItemMesh->SetWorldRotation(FRotator::ZeroRotator);
}

void ASword::AttachToSocketName(USceneComponent* InParent, const FName& InSocketName)
{

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}


void ASword::OnBoxOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorIsSameType(OtherActor)) return;

	FHitResult BoxHit;
	//The input to the function here for the hit result, by reference, so it will be changed within the function and passed back out
	BoxTrace(BoxHit);

	if (BoxHit.GetActor()) {
		if (ActorIsSameType(BoxHit.GetActor())) return;
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(),Damage,GetInstigator()->GetController(),this,UDamageType::StaticClass());
		ExecuteGetHit(BoxHit);
		CreateFields(BoxHit.ImpactPoint);
	}
}

bool ASword::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void ASword::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* InterfaceActor = Cast<IHitInterface>(BoxHit.GetActor());

	if (InterfaceActor)
	{
		InterfaceActor->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());

	}
}

void ASword::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxStartTrace->GetComponentLocation();
	const FVector End = BoxEndTrace->GetComponentLocation();

	//Template Class to dynamically change an array of pointers (of various different types should it need to be)

	TArray<AActor*> ActorsToIgnore;
\
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors) {
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxStartTrace->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowDebugBox ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
