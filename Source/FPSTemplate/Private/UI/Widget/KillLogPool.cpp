// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KillLogPool.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "OWGameplayTags.h"
#include "Message/OWMessageTypes.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamSubsystem.h"
#include "Components/ListView.h"
#include "UI/Widget/KillLog.h"

void UKillLogPool::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(WidgetController);

	// Get Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	// Listen Hero Killed Gameplay Tag
	FGameplayTag HeroKilledTag = FOWGameplayTags::Get().Gameplay_Message_HeroKilled;
	FGameplayMessageListenerHandle HealthPlateAddListener =
		GameplayMessageSubsystem.RegisterListener<FHeroKilledInfo>(HeroKilledTag, this, &UKillLogPool::OnHeroKilled);
}

void UKillLogPool::OnHeroKilled(FGameplayTag Channel, const FHeroKilledInfo& Payload)
{
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		AOWPlayerState* OwnerPlayerState = Cast<AOWPlayerState>(OverlayWidgetController->PlayerState);

		AOWPlayerState* InstigatorPlayerState = Cast<AOWPlayerState>(Payload.SourcePlayerState);
		AOWPlayerState* VictimPlayerState = Cast<AOWPlayerState>(Payload.TargetPlayerState);

		UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();

		// Check Instigator is on Same Team and Victim is on Different Team 
		if ((TeamSubsystem->CompareTeams(InstigatorPlayerState, OwnerPlayerState) == EOWTeamComparison::OnSameTeam) &&
			(TeamSubsystem->CompareTeams(VictimPlayerState, OwnerPlayerState) == EOWTeamComparison::DifferentTeams))
		{
			AddNewItem(EKillLogType::AllyKill, InstigatorPlayerState, VictimPlayerState);
		}
		// Check Instigator is on Different Team and Victim is on Same Team 
		else if ((TeamSubsystem->CompareTeams(InstigatorPlayerState, OwnerPlayerState) == EOWTeamComparison::DifferentTeams) &&
			(TeamSubsystem->CompareTeams(VictimPlayerState, OwnerPlayerState) == EOWTeamComparison::OnSameTeam))
		{
			AddNewItem(EKillLogType::EnemyKill, InstigatorPlayerState, VictimPlayerState);
		}
	}
}

void UKillLogPool::AddNewItem(EKillLogType KillLogType, const AOWPlayerState* InstigatorPlayerState, const AOWPlayerState* VictimPlayerState)
{
	// Check that ListView and List Item Class are set
	if (!ListView_KillLog || !ListItemClass)
	{
		return;
	}

	// Remove the Oldest Item if the Number of items in the ListView is at its maximum
	TArray<UObject*> CurrentItems = ListView_KillLog->GetListItems();
	if (CurrentItems.Num() >= MaxItems)
	{
		if (UKillLog* OldestKillLog = Cast<UKillLog>(CurrentItems[0]))
		{
			OldestKillLog->OnKillLogFinished.BindLambda([this](UKillLog* InKillLog)
				{
					ListView_KillLog->RemoveItem(InKillLog);
				});
			OldestKillLog->HoldThenPlayEnd();
		}
	}

	// Create New Combat Log Listening Gameplay Message 
	UKillLog* NewKillLog = NewObject<UKillLog>(this);
	NewKillLog->ShowKillLog(KillLogType, InstigatorPlayerState, VictimPlayerState);

	NewKillLog->OnKillLogFinished.BindLambda([this](UKillLog* InKillLog)
		{
			ListView_KillLog->RemoveItem(InKillLog);
		});

	ListView_KillLog->AddItem(NewKillLog);
}

