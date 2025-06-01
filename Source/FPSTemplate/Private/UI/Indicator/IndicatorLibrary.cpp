// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Indicator/IndicatorLibrary.h"
#include "Component/IndicatorManagerComponent.h"

UIndicatorLibrary::UIndicatorLibrary()
{

}

UIndicatorManagerComponent* UIndicatorLibrary::GetIndicatorManagerComponent(AController* Controller)
{
	return UIndicatorManagerComponent::GetComponent(Controller); 
}
