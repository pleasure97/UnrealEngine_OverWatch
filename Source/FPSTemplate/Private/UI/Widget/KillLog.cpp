#include "UI/Widget/KillLog.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"

void UKillLog::NativeConstruct()
{
	Super::NativeConstruct(); 

    if (StartAnimation)
    {
        FWidgetAnimationDynamicEvent AnimationFinishedEvent;
        AnimationFinishedEvent.BindUFunction(this, FName("OnStartAnimationFinished"));
        BindToAnimationFinished(StartAnimation, AnimationFinishedEvent);
    }

    if (EndAnimation)
    {
        FWidgetAnimationDynamicEvent AnimationFinishedEvent;
        AnimationFinishedEvent.BindUFunction(this, FName("OnEndAnimationFinished"));
        BindToAnimationFinished(EndAnimation, AnimationFinishedEvent);
    }
}

void UKillLog::NativeDestruct()
{
    if (StartAnimation)
    {
        UnbindAllFromAnimationFinished(StartAnimation);
    }

    if (EndAnimation)
    {
        UnbindAllFromAnimationFinished(EndAnimation);
    }

    if (HoldTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
    }

	Super::NativeDestruct(); 
}

void UKillLog::ShowKillLog(EKillLogType KillLogType, const AOWPlayerState* InstigatorPlayerState, const AOWPlayerState* VictimPlayerState)
{
    // TODO - Broadcast Skill Info 
    Border_KillSkill->SetVisibility(ESlateVisibility::Collapsed); 

    // Get Instigator and Victim Hero Infos
    FOWHeroInfo InstigatorHeroInfo;
    FOWHeroInfo VictimHeroInfo;

    UOWAbilitySystemLibrary::GetIndividualHeroInfo(this, InstigatorPlayerState->GetHeroName(), InstigatorHeroInfo);
    UOWAbilitySystemLibrary::GetIndividualHeroInfo(this, VictimPlayerState->GetHeroName(), VictimHeroInfo);

    TextBlock_Kill->SetText(FText::FromString(InstigatorPlayerState->GetPlayerName()));
    UTexture2D* InstigatorPortrait = const_cast<UTexture2D*>(InstigatorHeroInfo.HeroPortrait2D.Get());
    Portrait_Kill->SetBrushFromTexture(InstigatorPortrait, true);

    TextBlock_Dead->SetText(FText::FromString(VictimPlayerState->GetPlayerName()));
    UTexture2D* VictimPortrait = const_cast<UTexture2D*>(VictimHeroInfo.HeroPortrait2D.Get());
    Portrait_Dead->SetBrushFromTexture(VictimPortrait, true); 

    switch (KillLogType)
    {
    // When Ally Kill Enemy 
    case EKillLogType::AllyKill:
    {
        Border_Kill->SetBrushColor(KillLogColors::Blue); 
        Border_Dead->SetBrushColor(KillLogColors::Red); 
        break;
    }
    // When Enemy Kill Ally 
    case EKillLogType::EnemyKill:
    {
        Border_Kill->SetBrushColor(KillLogColors::Red);
        Border_Dead->SetBrushColor(KillLogColors::Blue);
        break; 
    }
    }
}

void UKillLog::OnStartAnimationFinished()
{
    GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this, &UKillLog::HoldThenPlayEnd, 5.f, false);
}

void UKillLog::OnEndAnimationFinished()
{
    if (OnKillLogFinished.IsBound())
    {
        OnKillLogFinished.Execute(this);
    }
}

void UKillLog::HoldThenPlayEnd()
{
    PlayAnimation(EndAnimation);
}
