// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealInterface : public UInterface
{
	GENERATED_BODY()
};

class AHealPickup;
class SLASH_API IHealInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReceiveHealingPickUp(AHealPickup* Heal);

};
