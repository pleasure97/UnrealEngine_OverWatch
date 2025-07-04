#include "UI/Widget/CombatLog.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/Data/KillFeedEntry.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamSubsystem.h"

void UCombatLog::NativeDestruct()
{
	if (HoldTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle); 
	}

	Super::NativeDestruct();
}

void UCombatLog::NativeOnListItemObjectSet(UObject* ListItem)
{
	if (UKillFeedEntry* KillFeedEntry = Cast<UKillFeedEntry>(ListItem))
	{
		ShowCombatLog(KillFeedEntry->Message);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Kill Feed Entry is not casted in UKillLog::NativeOnListItemObjectSet()"));
	}
}

void UCombatLog::ShowCombatLog(FHeroKilledInfo& HeroKilledInfo)
{
	AOWPlayerState* InstigatorPlayerState = Cast<AOWPlayerState>(HeroKilledInfo.InstigatorPlayerState); 
	AOWPlayerState* TargetPlayerState = Cast<AOWPlayerState>(HeroKilledInfo.TargetPlayerState);

	if (InstigatorPlayerState == GetOwningPlayerState())
	{
		ECombatLogType KillTypeLog = ECombatLogType::Kill;
		if (Border_CombatLog)
		{
			Border_CombatLog->SetBrushColor(CombatLogInfoMap[KillTypeLog].CombatLogColor);
		}
		if (Image_CombatLog)
		{
			Image_CombatLog->SetBrushFromTexture(CombatLogInfoMap[KillTypeLog].CombatLogIcon, true);
		}
		if (TextBlock_Username)
		{
			FString TargetPlayerName = TargetPlayerState->GetPlayerName();
			if (TargetPlayerName.Len() > 12)
			{
				TargetPlayerName = TargetPlayerName.Left(12);
				TextBlock_Username->SetText(FText::FromString(TargetPlayerName)); 
			}
		}
		if (TextBlock_AdditionalLog)
		{
			if (!CombatLogInfoMap[KillTypeLog].CombatLogText.IsEmpty())
			{
				TextBlock_AdditionalLog->SetVisibility(ESlateVisibility::Visible);
				TextBlock_AdditionalLog->SetText(CombatLogInfoMap[KillTypeLog].CombatLogText);
			}
			else
			{
				TextBlock_AdditionalLog->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	else if (TargetPlayerState == GetOwningPlayerState())
	{
		ECombatLogType DeathTypeLog = ECombatLogType::Death;

		if (Border_CombatLog)
		{
			Border_CombatLog->SetBrushColor(CombatLogInfoMap[DeathTypeLog].CombatLogColor);
		}
		if (Image_CombatLog)
		{
			Image_CombatLog->SetBrushFromTexture(CombatLogInfoMap[DeathTypeLog].CombatLogIcon, true);
		}
		if (TextBlock_Username)
		{
			FString InstigatorPlayerName = InstigatorPlayerState->GetPlayerName();
			if (InstigatorPlayerName.Len() > 12)
			{
				InstigatorPlayerName = InstigatorPlayerName.Left(12);
				TextBlock_Username->SetText(FText::FromString(InstigatorPlayerName));
			}
		}
		if (TextBlock_AdditionalLog)
		{
			if (!CombatLogInfoMap[DeathTypeLog].CombatLogText.IsEmpty())
			{
				TextBlock_AdditionalLog->SetVisibility(ESlateVisibility::Visible);
				TextBlock_AdditionalLog->SetText(CombatLogInfoMap[DeathTypeLog].CombatLogText);
			}
			else
			{
				TextBlock_AdditionalLog->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	else
	{
		return;
	}

	// Play Kill Log Animation 
	if (CombatLogAnimation)
	{
		PlayAnimation(CombatLogAnimation);
		GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this, &UCombatLog::OnLogExpired, 3.f, false);
	}
}

void UCombatLog::OnLogExpired()
{
	// Get Kill Log Entry as List Item Object 
	UObject* KillLogEntry = UUserObjectListEntryLibrary::GetListItemObject(this);
	if (KillLogEntry)
	{
		OnCombatLogExpired.Broadcast(KillLogEntry);
	}
}