// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthHUDComponent.h"
#include "HUD/HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthHUDComponent::SetHealthPercent(float Percent)
{

	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());

	}
	if (HealthBarWidget && HealthBarWidget->HealthBar) {

		HealthBarWidget->HealthBar->SetPercent(Percent);
	}

}

