// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/PickupInterface.h"

void IPickupInterface::ProcessHeal(AHealPickup* Heal)
{
}

void IPickupInterface::SetOverlappingItem(AItem* Item)
{
	// overriden in respective classes
}

void IPickupInterface::AddSouls(ASoul* Soul)
{
	// overriden in respective classes
}


void IPickupInterface::AddGold(ATreasure* Treasure)
{
	// overriden in respective classes
}
