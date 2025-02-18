// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/OWHUD.h"
#include "Player/OWPlayerState.h"
#include "UI/WidgetController/OWWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "Game/OWGameModeBase.h"
#include "Game/OWGameState.h"

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

void UOWAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, EHeroName HeroName, UAbilitySystemComponent* ASC, float Level)
{
	AActor* AvatarActor = ASC->GetAvatarActor(); 

	UHeroInfo* HeroInfo = GetHeroInfo(WorldContextObject); 
	const FOWHeroInfo& OWHeroInfo = HeroInfo->GetHeroDefaultInfo(HeroName);

	/*FGameplayEffectContextHandle DefensiveAttributesContextHandle = ASC->MakeEffectContext(); 
	DefensiveAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle DefensiveAttributeSpecHandle = ASC->MakeOutgoingSpec(OWHeroInfo.DefensiveAttributes, Level, DefensiveAttributesContextHandle); 
	ASC->ApplyGameplayEffectSpecToSelf(*DefensiveAttributeSpecHandle.Data.Get()); */
}

void UOWAbilitySystemLibrary::GiveDefaultAbilities(const UObject* WorldContextObject, EHeroName HeroName, UAbilitySystemComponent* ASC)
{
	UHeroInfo* HeroInfo = GetHeroInfo(WorldContextObject); 
	if (!HeroInfo) return; 

	for (TSubclassOf<UGameplayAbility> AbilityClass : HeroInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1); 
		ASC->GiveAbility(AbilitySpec); 
	}

	const FOWHeroInfo& OWHeroInfo = HeroInfo->GetHeroDefaultInfo(HeroName);
	for (const FOWAbilityInfo& AbilityInfo : OWHeroInfo.Abilities)
	{
		TSubclassOf<UGameplayAbility> AbilityClass = AbilityInfo.Ability; 
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1); 
		ASC->GiveAbility(AbilitySpec); 
	}
}

UHeroInfo* UOWAbilitySystemLibrary::GetHeroInfo(const UObject* WorldContextObject)
{
	const AOWGameModeBase* OWGameMode = Cast<AOWGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)); 
	if (OWGameMode == nullptr)
	{
		return nullptr; 
	}
	return OWGameMode->HeroInfo;
}

EHeroName UOWAbilitySystemLibrary::GetHeroName(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return EHeroName::None;

	if (World->GetNetMode() != NM_Client)
	{
		const AOWGameModeBase* OWGameMode = Cast<AOWGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
		return OWGameMode ? OWGameMode->SelectedHeroName : EHeroName::None; 
	}
	else
	{
		const AOWGameState* OWGameState = Cast<AOWGameState>(UGameplayStatics::GetGameState(WorldContextObject));
		return OWGameState ? OWGameState->SelectedHeroName : EHeroName::None;
	}
}
