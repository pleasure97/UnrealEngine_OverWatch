// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/CombatLogPool.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "OWGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/OWMessageTypes.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamSubsystem.h"
#include "Components/ListView.h"

void UCombatLogPool::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(WidgetController); 

	// Get Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	// Listen Hero Killed Gameplay Tag
	FGameplayTag HeroKilledTag = FOWGameplayTags::Get().Gameplay_Message_HeroKilled;
	FGameplayMessageListenerHandle HealthPlateAddListener =
		GameplayMessageSubsystem.RegisterListener<FHeroKilledInfo>(HeroKilledTag, this, &UCombatLogPool::OnHeroKilled);
}

void UCombatLogPool::OnHeroKilled(FGameplayTag Channel, const FHeroKilledInfo& Payload)
{
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		AOWPlayerState* OwnerPlayerState = Cast<AOWPlayerState>(OverlayWidgetController->PlayerState); 
		FString OwnerPlayerName = OwnerPlayerState->GetPlayerName(); 

		AOWPlayerState* SourcePlayerState = Cast<AOWPlayerState>(Payload.SourcePlayerState);
		AOWPlayerState* TargetPlayerState = Cast<AOWPlayerState>(Payload.TargetPlayerState);

		FString SourcePlayerName = SourcePlayerState->GetPlayerName();
		FString TargetPlayerName = TargetPlayerState->GetPlayerName();

		UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>(); 

		// Owner is Killed
		if (TargetPlayerName == OwnerPlayerName)
		{
			AddNewItem(ECombatLogType::Death, SourcePlayerName); 
		}
		// Other Hero on Diffrent Team is Killed
		else if ((TeamSubsystem->CompareTeams(SourcePlayerState, OwnerPlayerState) == EOWTeamComparison::OnSameTeam) &&
			(TeamSubsystem->CompareTeams(TargetPlayerState, OwnerPlayerState) == EOWTeamComparison::DifferentTeams))
		{
			AddNewItem(ECombatLogType::Kill, TargetPlayerName); 
		}
	}
}

void UCombatLogPool::AddNewItem(ECombatLogType CombatLogType, const FString& PlayerName)
{
	// Check that ListView and List Item Class are set
	if (!ListView_CombatLog || !ListItemClass)
	{
		return; 
	}

	// Remove the Oldest Item if the Number of items in the ListView is at its maximum
	TArray<UObject*> CurrentItems = ListView_CombatLog->GetListItems(); 
	if (CurrentItems.Num() >= MaxItems)
	{
		ListView_CombatLog->RemoveItem(CurrentItems[0]);
	}

	// Create New Combat Log Listening Gameplay Message 
	UCombatLog* CombatLog = NewObject<UCombatLog>(this);
	CombatLog->ShowCombatLog(CombatLogType, PlayerName); 
	ListView_CombatLog->AddItem(CombatLog);
	CombatLog->PlayAnimation(CombatLog->StartAnimation);
}
