// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KillLogPool.h"
#include "OWGameplayTags.h"
#include "Message/OWMessageTypes.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamSubsystem.h"
#include "Components/ListView.h"
#include "UI/Widget/KillLog.h"
#include "UI/Data/KillFeedEntry.h"

void UKillLogPool::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(WidgetController);

	// Get Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	// Listen Hero Killed Gameplay Tag
	FGameplayTag HeroKilledTag = FOWGameplayTags::Get().Gameplay_Message_KillFeed;
	HeroKilledListener = GameplayMessageSubsystem.RegisterListener<FHeroKilledInfo>(HeroKilledTag, this, &UKillLogPool::OnHeroKilled);
}

void UKillLogPool::NativeDestruct()
{
	if (HeroKilledListener.IsValid())
	{
		HeroKilledListener.Unregister(); 
	}

	Super::NativeDestruct(); 
}

void UKillLogPool::UpdateDisplayVisibility()
{
	if (ListView_KillLog)
	{
		if (ListView_KillLog->GetNumItems() > 0)
		{
			ListView_KillLog->SetVisibility(ESlateVisibility::Visible); 
		}
		else
		{
			ListView_KillLog->SetVisibility(ESlateVisibility::Collapsed); 
		}
	}
}

void UKillLogPool::OnHeroKilled(FGameplayTag Channel, const FHeroKilledInfo& Payload)
{
	UKillFeedEntry* KillFeedEntry = NewObject<UKillFeedEntry>(this);
	if (KillFeedEntry)
	{
		KillFeedEntry->Message = Payload;
		if (ListView_KillLog)
		{
			ListView_KillLog->AddItem(KillFeedEntry); 

			UpdateDisplayVisibility(); 
		}
	}
}

