// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/CombatLogPool.h"
#include "OWGameplayTags.h"
#include "Message/OWMessageTypes.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamSubsystem.h"
#include "Components/ListView.h"
#include "UI/Data/KillFeedEntry.h"

void UCombatLogPool::NativeConstruct()
{
	Super::NativeConstruct(); 

	ClearCombatLogPool(); 

	// Get Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	// Listen Hero Killed Gameplay Tag
	FGameplayTag HeroKilledTag = FOWGameplayTags::Get().Gameplay_Message_KillFeed;
	HeroKilledListener = GameplayMessageSubsystem.RegisterListener<FHeroKilledInfo>(HeroKilledTag, this, &UCombatLogPool::OnHeroKilled);
}

void UCombatLogPool::NativeDestruct()
{
	if (HeroKilledListener.IsValid())
	{
		HeroKilledListener.Unregister();
	}

	ClearCombatLogPool();

	Super::NativeDestruct(); 
}

void UCombatLogPool::OnHeroKilled(FGameplayTag Channel, const FHeroKilledInfo& Payload)
{
	UKillFeedEntry* KillFeedEntry = NewObject<UKillFeedEntry>(this);
	if (KillFeedEntry)
	{
		KillFeedEntry->Message = Payload;
		if (ListView_CombatLog)
		{
			ListView_CombatLog->AddItem(KillFeedEntry);

			UpdateDisplayVisibility();
		}
	}
}

void UCombatLogPool::ClearCombatLogPool()
{
	if (ListView_CombatLog)
	{
		ListView_CombatLog->ClearListItems(); 
	}
}

void UCombatLogPool::UpdateDisplayVisibility()
{
	if (ListView_CombatLog)
	{
		if (ListView_CombatLog->GetNumItems() > 0)
		{
			ListView_CombatLog->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ListView_CombatLog->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
