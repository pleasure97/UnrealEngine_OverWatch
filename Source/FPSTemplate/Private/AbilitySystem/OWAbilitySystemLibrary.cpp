// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/OWHUD.h"
#include "Player/OWPlayerState.h"
#include "UI/WidgetController/OWWidgetController.h"

bool UOWAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AOWHUD*& OutOWHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutOWHUD = Cast<AOWHUD>(PC->GetHUD()); 
		if (OutOWHUD)
		{
			AOWPlayerState* PS = PC->GetPlayerState<AOWPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent(); 
			UAttributeSet* AS = PS->GetAttributeSet(); 

			OutWCParams.PlayerController = PC; 
			OutWCParams.PlayerState = PS; 
			OutWCParams.AbilitySystemComponent = ASC; 
			OutWCParams.AttributeSet = AS; 

			return true; 
		}
	}
	return false;
}

UOverlayWidgetController* UOWAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams; 
	AOWHUD* OWHUD = nullptr; 
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, OWHUD))
	{
		return OWHUD->GetOverlayWidgetController(WidgetControllerParams); 
	}

	return nullptr;
}
