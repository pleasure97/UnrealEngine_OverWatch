// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OWWidgetController.h"
#include "Player/OWPlayerController.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "Game/OWGameState.h"

void UOWWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController; 
	PlayerState = WCParams.PlayerState; 
	AbilitySystemComponent = WCParams.AbilitySystemComponent; 
	AttributeSet = WCParams.AttributeSet; 
}

void UOWWidgetController::BroadcastInitialValues()
{
}

void UOWWidgetController::BindCallbacksToDependencies()
{
}

AOWPlayerController* UOWWidgetController::GetOW_PC()
{
	if (OWPlayerController == nullptr)
	{
		OWPlayerController = Cast<AOWPlayerController>(PlayerController); 
	}
	return OWPlayerController; 
}

AOWPlayerState* UOWWidgetController::GetOW_PS()
{
	if (OWPlayerState == nullptr)
	{
		OWPlayerState = Cast<AOWPlayerState>(PlayerState); 
	}

	return OWPlayerState; 
}

UOWAbilitySystemComponent* UOWWidgetController::GetOW_ASC()
{
	if (OWAbilitySystemComponent == nullptr)
	{
		OWAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(AbilitySystemComponent); 
	}

	return OWAbilitySystemComponent; 
}

UOWAttributeSet* UOWWidgetController::GetOW_AS()
{
	if (OWAttributeSet == nullptr)
	{
		OWAttributeSet = Cast<UOWAttributeSet>(AttributeSet); 
	}

	return OWAttributeSet; 
}