// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OWGamePhaseSubsystem.h"
#include "AbilitySystem/Abilities/OWGamePhaseAbility.h"
#include "GameFramework/GameStateBase.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"

bool UOWGamePhaseSubsystem::IsPhaseActive(const FGameplayTag& GamePhaseTag) const
{
	for (const TPair<FGameplayAbilitySpecHandle, FOWGamePhaseEntry>& ActivePhasePair: ActivePhaseMap)
	{
		const FOWGamePhaseEntry& GamePhaseEntry = ActivePhasePair.Value; 
		if (GamePhaseEntry.GamePhaseTag.MatchesTag(GamePhaseTag))
		{
			return true; 
		}
	}
	return false; 
}

void UOWGamePhaseSubsystem::StartPhase(TSubclassOf<UOWGamePhaseAbility> GamePhaseAbility, const FOWGamePhaseDynamicDelegate& PhaseEndedDelegate)
{
	// Bind Phased Ended Delegate to Activate Subsequent Phase Ability 
	const FOWGamePhaseDelegate& EndedDelegate =
		FOWGamePhaseDelegate::CreateWeakLambda(const_cast<UObject*>(PhaseEndedDelegate.GetUObject()),
			[PhaseEndedDelegate](const UOWGamePhaseAbility* PhaseAbility)
			{
				PhaseEndedDelegate.ExecuteIfBound(PhaseAbility);
			}); 

	if (GetWorld())
	{
		// Get Custom Ability System Component From Custom Game State 
		UOWAbilitySystemComponent* GameStateASC = GetWorld()->GetGameState()->FindComponentByClass<UOWAbilitySystemComponent>(); 
		if (GameStateASC)
		{
			// Initialization - Ability, Ability Level, Input ID, Source Object)
			FGameplayAbilitySpec GamePhaseSpec(GamePhaseAbility, 1, 0, this); 
			FGameplayAbilitySpecHandle SpecHandle = GameStateASC->GiveAbilityAndActivateOnce(GamePhaseSpec); 
			FGameplayAbilitySpec* FoundSpec = GameStateASC->FindAbilitySpecFromHandle(SpecHandle); 

			if (FoundSpec && FoundSpec->IsActive())
			{
				// Register Game Phase Entry (Game Phase Tag, Game Phase Ended Callback) to ActivePhaseMap
				FOWGamePhaseEntry& GamePhaseEntry = ActivePhaseMap.FindOrAdd(SpecHandle); 
				GamePhaseEntry.PhaseEndedCallback = EndedDelegate;
			}
			else
			{
				EndedDelegate.ExecuteIfBound(nullptr);
			}
		}
	}
}

void UOWGamePhaseSubsystem::OnBeginPhase(const UOWGamePhaseAbility* GamePhaseAbility, const FGameplayAbilitySpecHandle GamePhaseAbilitySpecHandle)
{
	const FGameplayTag& IncomingGamePhaseTag = GamePhaseAbility->GetGamePhaseTag(); 

	UE_LOG(LogTemp, Log, TEXT("Beginning Phase '%s' (%s)"), *IncomingGamePhaseTag.ToString(), *GetNameSafe(GamePhaseAbility));

	if (GetWorld())
	{
		UOWAbilitySystemComponent* GameStateASC = GetWorld()->GetGameState()->FindComponentByClass<UOWAbilitySystemComponent>();
		if (GameStateASC)
		{
			TArray<FGameplayAbilitySpec*> ActivePhases;
			for (const TPair<FGameplayAbilitySpecHandle, FOWGamePhaseEntry>& ActivePhasePair : ActivePhaseMap)
			{
				const FGameplayAbilitySpecHandle ActiveAbilitySpecHandle = ActivePhasePair.Key;
				if (FGameplayAbilitySpec* GameplayAbilitySpec = GameStateASC->FindAbilitySpecFromHandle(ActiveAbilitySpecHandle))
				{
					ActivePhases.Add(GameplayAbilitySpec);
				}
			}

			for (const FGameplayAbilitySpec* ActivePhase : ActivePhases)
			{
				const UOWGamePhaseAbility* ActiveGamePhaseAbility = CastChecked<UOWGamePhaseAbility>(ActivePhase->Ability);
				const FGameplayTag& ActiveGamePhaseTag = ActiveGamePhaseAbility->GetGamePhaseTag();
				if (!IncomingGamePhaseTag.MatchesTag(ActiveGamePhaseTag))
				{
					UE_LOG(LogTemp, Log, TEXT("Ending Phase '%s' (%s)"), *ActiveGamePhaseTag.ToString(), *GetNameSafe(ActiveGamePhaseAbility));

					FGameplayAbilitySpecHandle HandleToEnd = ActivePhase->Handle;

					GameStateASC->CancelAbilitiesByFunc([HandleToEnd](const UGameplayAbility* OWAbility, FGameplayAbilitySpecHandle Handle)
						{
							return Handle == HandleToEnd;
						}, true);
				}
			}

			FOWGamePhaseEntry& GamePhaseEntry = ActivePhaseMap.FindOrAdd(GamePhaseAbilitySpecHandle);
			GamePhaseEntry.GamePhaseTag = IncomingGamePhaseTag;
		}
	}
}

void UOWGamePhaseSubsystem::OnEndPhase(const UOWGamePhaseAbility* GamePhaseAbility, const FGameplayAbilitySpecHandle GamePhaseAbilitySpecHandle)
{
	const FGameplayTag& EndedGamePhaseTag = GamePhaseAbility->GetGamePhaseTag(); 
	UE_LOG(LogTemp, Log, TEXT("Ended Phase '%s' (%s)"), *EndedGamePhaseTag.ToString(), *GetNameSafe(GamePhaseAbility));

	const FOWGamePhaseEntry& GamePhaseEntry = ActivePhaseMap.FindChecked(GamePhaseAbilitySpecHandle); 
	GamePhaseEntry.PhaseEndedCallback.ExecuteIfBound(GamePhaseAbility); 

	ActivePhaseMap.Remove(GamePhaseAbilitySpecHandle); 
}
