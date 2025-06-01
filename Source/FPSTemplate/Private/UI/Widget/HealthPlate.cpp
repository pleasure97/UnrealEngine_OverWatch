// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthPlate.h"
#include "UI/Indicator/IndicatorController.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "Components/TextBlock.h"
#include "UI/Widget/HealthBarPool.h"
#include "Character/OWCharacterBase.h"
#include "Team/AsyncAction_ObserveTeam.h"
#include "Team/OWTeamSubsystem.h"
#include "Components/Image.h"

void UHealthPlate::BindIndicator_Implementation(UIndicatorController* Indicator)
{
	if (!Indicator)
	{
		return;
	}

	IndicatorController = Indicator; 

	IndicatorController->SetDesiredVisibility(false);

	APawn* Pawn = Cast<APawn>(Indicator->GetDataObject()); 
	if (Pawn)
	{
		APlayerState* PlayerState = Pawn->GetPlayerState();
		if (PlayerState)
		{
			OWPlayerState = Cast<AOWPlayerState>(PlayerState);
		}
	}

	if (OWPlayerState)
	{
		if (AController* PlayerController = Cast<AController>(OWPlayerState->GetOwner()))
		{
			if (PlayerController)
			{
				if (AOWCharacterBase* OWCharacterBase = Cast<AOWCharacterBase>(PlayerController->GetPawn()))
				{
					OWCharacterBase->OnASCRegistered.AddUObject(this, &UHealthPlate::OnPawnReady); 
					OWCharacterBase->OnDeath.AddDynamic(this, &UHealthPlate::OnDeath); 
				}
			}
		}
	}
}

void UHealthPlate::UnbindIndicator_Implementation(UIndicatorController* Indicator)
{
	if (OWPlayerState)
	{
		if (AController* PlayerController = Cast<AController>(OWPlayerState->GetOwner()))
		{
			if (PlayerController)
			{
				if (AOWCharacterBase* OWCharacterBase = Cast<AOWCharacterBase>(PlayerController->GetPawn()))
				{
					OWCharacterBase->OnASCRegistered.RemoveAll(this); 
					OWCharacterBase->OnDeath.RemoveAll(this); 
				}
			}
		}

		UAbilitySystemComponent* AbilitySystemComponent = OWPlayerState->GetAbilitySystemComponent();
		UOWAttributeSet* OWAttributeSet = Cast<UOWAttributeSet>(OWPlayerState->GetAttributeSet());
		if (AbilitySystemComponent && OWAttributeSet)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				OWAttributeSet->GetMaxHealthAttribute()).RemoveAll(this); 

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				OWAttributeSet->GetMaxArmorAttribute()).RemoveAll(this);

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				OWAttributeSet->GetMaxShieldAttribute()).RemoveAll(this);

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				OWAttributeSet->GetHealthAttribute()).RemoveAll(this);

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				OWAttributeSet->GetArmorAttribute()).RemoveAll(this);

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				OWAttributeSet->GetShieldAttribute()).RemoveAll(this);

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				OWAttributeSet->GetOverHealthAttribute()).RemoveAll(this);
		}
	}
	UAsyncAction_ObserveTeam* AsyncAction_ObserveTeam = UAsyncAction_ObserveTeam::ObserveTeam(OWPlayerState);
	if (AsyncAction_ObserveTeam)
	{
		AsyncAction_ObserveTeam->OnTeamChanged.RemoveAll(this); 
	}

	OWPlayerState = nullptr; 
	IndicatorController = nullptr; 
}

void UHealthPlate::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (WBP_HealthBarPool)
	{
		WBP_HealthBarPool->UpdateHealthBars(Data.NewValue);
	}
}

void UHealthPlate::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	if (WBP_HealthBarPool)
	{
		WBP_HealthBarPool->UpdateMaxHealthBars(Data.NewValue);
	}
}

void UHealthPlate::OnArmorChanged(const FOnAttributeChangeData& Data)
{
	if (WBP_HealthBarPool)
	{
		WBP_HealthBarPool->UpdateArmorBars(Data.NewValue);
	}
}

void UHealthPlate::OnMaxArmorChanged(const FOnAttributeChangeData& Data)
{
	if (WBP_HealthBarPool)
	{
		WBP_HealthBarPool->UpdateMaxArmorBars(Data.NewValue);
	}
}

void UHealthPlate::OnShieldChanged(const FOnAttributeChangeData& Data)
{
	if (WBP_HealthBarPool)
	{
		WBP_HealthBarPool->UpdateShieldBars(Data.NewValue);
	}
}

void UHealthPlate::OnMaxShieldChanged(const FOnAttributeChangeData& Data)
{
	if (WBP_HealthBarPool)
	{
		WBP_HealthBarPool->UpdateMaxShieldBars(Data.NewValue);
	}
}

void UHealthPlate::OnOverHealthChanged(const FOnAttributeChangeData& Data)
{
	if (WBP_HealthBarPool)
	{
		WBP_HealthBarPool->UpdateOverHealthBars(Data.NewValue);
	}
}

void UHealthPlate::OnPawnReady(UAbilitySystemComponent* ASC)
{
	if (IndicatorController)
	{
		IndicatorController->SetDesiredVisibility(true);
	}
	SetVisibility(ESlateVisibility::Visible); 

	// Initialize Player Name
	if (TextBlock_PlayerName && OWPlayerState)
	{
		TextBlock_PlayerName->SetText(FText::FromString(OWPlayerState->GetPlayerName()));
	}

	// Update Team Visuals 
	UAsyncAction_ObserveTeam* AsyncAction_ObserveTeam = UAsyncAction_ObserveTeam::ObserveTeam(OWPlayerState);
	if (AsyncAction_ObserveTeam)
	{
		AsyncAction_ObserveTeam->OnTeamChanged.AddDynamic(this, &UHealthPlate::SetTeamVisuals);
	}

	// Bind Attribute Delegates 
	UOWAttributeSet* OWAttributeSet = Cast<UOWAttributeSet>(OWPlayerState->GetAttributeSet());
	if (ASC && OWAttributeSet)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(
			OWAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UHealthPlate::OnMaxHealthChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(
			OWAttributeSet->GetMaxArmorAttribute()).AddUObject(this, &UHealthPlate::OnMaxArmorChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(
			OWAttributeSet->GetMaxShieldAttribute()).AddUObject(this, &UHealthPlate::OnMaxShieldChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(
			OWAttributeSet->GetHealthAttribute()).AddUObject(this, &UHealthPlate::OnHealthChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(
			OWAttributeSet->GetArmorAttribute()).AddUObject(this, &UHealthPlate::OnArmorChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(
			OWAttributeSet->GetShieldAttribute()).AddUObject(this, &UHealthPlate::OnShieldChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(
			OWAttributeSet->GetOverHealthAttribute()).AddUObject(this, &UHealthPlate::OnOverHealthChanged);
	}
	
}

void UHealthPlate::OnDeath(AActor* DeadActor)
{
	IndicatorController->UnregisterIndicator(); 
}

void UHealthPlate::SetTeamVisuals(bool bTeamSet, int32 TeamID)
{

}

void UHealthPlate::UpdateVisibility()
{
	// TODO - UpdateVisibility() if Enemy IsInView() in Tick()?
	if (OWPlayerState->GetPlayerController() == GetOwningPlayer())
	{
		SetVisibility(ESlateVisibility::Collapsed); 
	}
	else
	{
		UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>(); 
		if (TeamSubsystem)
		{
			EOWTeamComparison TeamComparison = TeamSubsystem->CompareTeams(OWPlayerState, GetOwningPlayer()); 
			switch (TeamComparison)
			{
			case EOWTeamComparison::OnSameTeam:
				if (WBP_HealthBarPool)
				{
					WBP_HealthBarPool->SetHealthBarColor(HealthPlateColors::White);
				}
				if (TextBlock_PlayerName)
				{
					TextBlock_PlayerName->SetColorAndOpacity(HealthPlateColors::Blue);
				}
				if (Image_PlayerArrow)
				{
					Image_PlayerArrow->SetVisibility(ESlateVisibility::Visible);
				}
				return; 
			case EOWTeamComparison::DifferentTeams:
				if (WBP_HealthBarPool)
				{
					WBP_HealthBarPool->SetHealthBarColor(HealthPlateColors::Red); 
				}
				if (TextBlock_PlayerName)
				{
					TextBlock_PlayerName->SetColorAndOpacity(HealthPlateColors::Red);
				}
				if (Image_PlayerArrow)
				{
					Image_PlayerArrow->SetVisibility(ESlateVisibility::Collapsed); 
				}
				return;
			case EOWTeamComparison::InvalidArgument:
				SetVisibility(ESlateVisibility::Collapsed); 
				return;
			}
		}
	}
}