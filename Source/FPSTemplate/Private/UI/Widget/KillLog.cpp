#include "UI/Widget/KillLog.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "UI/Data/KillFeedEntry.h"
#include "Team/OWTeamSubsystem.h"

void UKillLog::NativeDestruct()
{
    if (HoldTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
    }

	Super::NativeDestruct(); 
}

void UKillLog::NativeOnListItemObjectSet(UObject* ListItem)
{
    if (UKillFeedEntry* KillFeedEntry = Cast<UKillFeedEntry>(ListItem))
    {
        ShowKillLog(KillFeedEntry->Message);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Kill Feed Entry is not casted in UKillLog::NativeOnListItemObjectSet()")); 
    }
}

void UKillLog::ShowKillLog(FHeroKilledInfo& HeroKilledInfo)
{
    if (Border_KillSkill)
    {
        if (HeroKilledInfo.InstigatorTags.IsEmpty())
        {
            Border_KillSkill->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            // TODO - Can get skill or attack gameplay ability info from Instigator Tags?
            // Border_KillSkill->SetVisibility(ESlateVisibility::HitTestInvisible);
        }
    }

    // Get Instigator and Victim Hero Infos
    AOWPlayerState* InstigatorPlayerState = Cast<AOWPlayerState>(HeroKilledInfo.InstigatorPlayerState); 
    AOWPlayerState* TargetPlayerState = Cast<AOWPlayerState>(HeroKilledInfo.TargetPlayerState); 

    // Declare Instigator and Victim Hero Info 
    FOWHeroInfo InstigatorHeroInfo; 
    FOWHeroInfo VictimHeroInfo;

    if (InstigatorPlayerState && TargetPlayerState)
    {
        // Assign Instigator and Victim Hero Info 
        UOWAbilitySystemLibrary::GetIndividualHeroInfo(GetOwningPlayer(), InstigatorPlayerState->GetHeroName(), InstigatorHeroInfo);
        UOWAbilitySystemLibrary::GetIndividualHeroInfo(GetOwningPlayer(), TargetPlayerState->GetHeroName(), VictimHeroInfo);

        if (TextBlock_Kill)
        {
            FString InstigatorPlayerName = InstigatorPlayerState->GetPlayerName(); 
            // Clamp Instigator Player Name to 12 characters
            if (InstigatorPlayerName.Len() > 12)
            {
                InstigatorPlayerName = InstigatorPlayerName.Left(12); 
            }
            TextBlock_Kill->SetText(FText::FromString(InstigatorPlayerName));
        }
        if (Portrait_Kill)
        {
            UTexture2D* InstigatorPortrait = const_cast<UTexture2D*>(InstigatorHeroInfo.HeroPortrait2D.Get());
            Portrait_Kill->SetBrushFromTexture(InstigatorPortrait, true);
        }

        if (TextBlock_Dead)
        {
            FString TargetPlayerName = TargetPlayerState->GetPlayerName();
            // Clamp Target Player Name to 12 characters
            if (TargetPlayerName.Len() > 12)
            {
                TargetPlayerName = TargetPlayerName.Left(12);
            }
            TextBlock_Dead->SetText(FText::FromString(TargetPlayerName));
        }

        if (Portrait_Dead)
        {
            UTexture2D* VictimPortrait = const_cast<UTexture2D*>(VictimHeroInfo.HeroPortrait2D.Get());
            Portrait_Dead->SetBrushFromTexture(VictimPortrait, true);
        }

        // Get Team Subsystem 
        if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
        {
            // Compare Teams - Ally Kill
            if ((TeamSubsystem->CompareTeams(GetOwningPlayerState(), InstigatorPlayerState) == EOWTeamComparison::OnSameTeam)
                && (TeamSubsystem->CompareTeams(GetOwningPlayer(), TargetPlayerState) == EOWTeamComparison::DifferentTeams))
            {
                if (Border_Kill)
                {
                    Border_Kill->SetBrushColor(KillLogColors::Blue);
                }
                if (Border_Dead)
                {
                    Border_Dead->SetBrushColor(KillLogColors::Red);
                }
            }
            // Compare Teams - Enemy Kill 
            else if ((TeamSubsystem->CompareTeams(GetOwningPlayerState(), InstigatorPlayerState) == EOWTeamComparison::DifferentTeams)
                && (TeamSubsystem->CompareTeams(GetOwningPlayer(), TargetPlayerState) == EOWTeamComparison::OnSameTeam))
            {
                if (Border_Kill)
                {
                    Border_Kill->SetBrushColor(KillLogColors::Red);
                }
                if (Border_Dead)
                {
                    Border_Dead->SetBrushColor(KillLogColors::Blue);
                }
            }
            // Compare Teams - Invalid 
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Find invalid team relationship when Comparing Teams in UKillLog::ShowKillLog()")); 
                return;
            }

            // Play Kill Log Animation 
            if (KillLogAnimation)
            {
                PlayAnimation(KillLogAnimation);
                GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this, &UKillLog::OnLogExpired, 5.f, false);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Instigator and Target Player State is not casted in UKillLog::ShowKillLog()"));
    }
}

void UKillLog::OnLogExpired()
{
    // Get Kill Log Entry as List Item Object 
    UObject* KillLogEntry = UUserObjectListEntryLibrary::GetListItemObject(this); 
    if (KillLogEntry)
    {
        OnKillLogExpired.Broadcast(KillLogEntry);
    }
}

