// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/IndicatorManagerComponent.h"
#include "UI/Indicator/IndicatorController.h"

UIndicatorManagerComponent::UIndicatorManagerComponent()
{
	// Auto Register to Enable Tick(), BeginPlay(), Replication,... etc
	bAutoRegister = true; 
	// Auto Activate to Enable Broadcasting OnComponentActivated
	bAutoActivate = true; 
}

UIndicatorManagerComponent* UIndicatorManagerComponent::GetComponent(AController* Controller)
{
	if (Controller)
	{
		return Controller->FindComponentByClass<UIndicatorManagerComponent>(); 
	}
	
	return nullptr;
}

void UIndicatorManagerComponent::AddIndicator(UIndicatorController* Indicator)
{
	Indicator->SetIndicatorManagerComponent(this); 
	OnIndicatorAdded.Broadcast(Indicator); 
	Indicators.Add(Indicator); 
}

void UIndicatorManagerComponent::RemoveIndicator(UIndicatorController* Indicator)
{
	if (Indicator)
	{
		ensure(Indicator->GetIndicatorMangerComponent() == this);

		OnIndicatorRemoved.Broadcast(Indicator); 
		Indicators.Remove(Indicator); 
	}
}

