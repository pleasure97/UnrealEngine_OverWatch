// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OWGamePhaseSubsystem.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
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

void UOWGamePhaseSubsystem::StartPhase(TSubclassOf<UOWGamePhaseAbility> GamePhaseAbility, FOWGamePhaseDelegate PhaseEndedDelegate)
{
	if (UWorld* World = GetWorld())
	{
		// Get Custom Ability System Component From Custom Game State 
		if (UOWAbilitySystemComponent* GameStateASC = World->GetGameState()->FindComponentByClass<UOWAbilitySystemComponent>())
		{
			// Initialization - Ability, Ability Level, Input ID, Source Object)
			FGameplayAbilitySpec GamePhaseSpec(GamePhaseAbility, 1, 0, this); 
			FGameplayAbilitySpecHandle SpecHandle = GameStateASC->GiveAbilityAndActivateOnce(GamePhaseSpec); 
			FGameplayAbilitySpec* FoundSpec = GameStateASC->FindAbilitySpecFromHandle(SpecHandle); 

			if (FoundSpec && FoundSpec->IsActive())
			{
				// Register Game Phase Entry (Game Phase Tag, Game Phase Ended Callback) to ActivePhaseMap
				FOWGamePhaseEntry& GamePhaseEntry = ActivePhaseMap.FindOrAdd(SpecHandle); 
				GamePhaseEntry.PhaseEndedCallback = PhaseEndedDelegate;
			}
			else
			{
				PhaseEndedDelegate.ExecuteIfBound(nullptr);
			}
		}
	}
}

void UOWGamePhaseSubsystem::WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FOWGamePhaseTagDelegate& WhenPhaseActive)
{
	// Declare PhaseObserver and Set Phase Information 
	FPhaseObserver PhaseObserver; 
	PhaseObserver.PhaseTag = PhaseTag; 
	PhaseObserver.MatchType = MatchType; 
	PhaseObserver.PhaseCallback = WhenPhaseActive; 

	// Add it to Phase Start Observers Array 
	PhaseStartObservers.Add(PhaseObserver); 

	// Check if Phase Tag is Active 
	if (IsPhaseActive(PhaseTag))
	{
		// Call Phase Active Callback
		WhenPhaseActive.ExecuteIfBound(PhaseTag); 
	}
}

void UOWGamePhaseSubsystem::WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FOWGamePhaseTagDelegate& WhenPhaseEnd)
{
	// Declare PhaseObserver and Set Phase Information 
	FPhaseObserver PhaseObserver;
	PhaseObserver.PhaseTag = PhaseTag;
	PhaseObserver.MatchType = MatchType;
	PhaseObserver.PhaseCallback = WhenPhaseEnd;

	// Add it to Phase End Observers Array 
	PhaseEndObservers.Add(PhaseObserver);
}

void UOWGamePhaseSubsystem::OnBeginPhase(const UOWGamePhaseAbility* GamePhaseAbility, const FGameplayAbilitySpecHandle GamePhaseAbilitySpecHandle)
{
	const FGameplayTag& IncomingGamePhaseTag = GamePhaseAbility->GetGamePhaseTag(); 

	UE_LOG(LogTemp, Log, TEXT("Beginning Phase '%s' (%s)"), *IncomingGamePhaseTag.ToString(), *GetNameSafe(GamePhaseAbility));

	if (UWorld* World = GetWorld())
	{
		if (UOWAbilitySystemComponent* GameStateASC = World->GetGameState()->FindComponentByClass<UOWAbilitySystemComponent>())
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

			// Notify All observers of this phase that it has started. 
			for (const FPhaseObserver& PhaseStartObserver : PhaseStartObservers)
			{
				if (PhaseStartObserver.IsMatch(IncomingGamePhaseTag))
				{
					PhaseStartObserver.PhaseCallback.ExecuteIfBound(IncomingGamePhaseTag); 
				}
			}
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

	// Notify all observers of this phase that it has ended.
	for (const FPhaseObserver& PhaseEndObserver : PhaseEndObservers)
	{
		if (PhaseEndObserver.IsMatch(EndedGamePhaseTag))
		{
			PhaseEndObserver.PhaseCallback.ExecuteIfBound(EndedGamePhaseTag);
		}
	}
}
