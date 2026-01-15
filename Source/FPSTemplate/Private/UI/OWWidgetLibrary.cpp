// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OWWidgetLibrary.h"
#include "UI/WidgetController/OWWidgetController.h"
#include "UI/HUD/OWHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Player/OWPlayerState.h"
#include "Components/TextBlock.h"

/* Widget Controller */
bool UOWWidgetLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AOWHUD*& OutOWHUD)
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

UOverlayWidgetController* UOWWidgetLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AOWHUD* OWHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, OWHUD))
	{
		return OWHUD->GetOverlayWidgetController(WidgetControllerParams);
	}

	return nullptr;
}

void UOWWidgetLibrary::UpdatePureNumberText(UTextBlock* TextBlock, float TextBlockValue)
{
	if (IsValid(TextBlock))
	{
		FNumberFormattingOptions Options;
		Options.UseGrouping = false; 

		FText PureText = FText::AsNumber(FMath::TruncToInt(TextBlockValue), &Options);
		TextBlock->SetText(PureText);
	}
}


