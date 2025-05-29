// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/TeamMemberInfoList.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Player/OWPlayerState.h"
#include "Game/OWGameState.h"
#include "UI/Widget/HeroSelection/TeamMemberInfo.h"

void UTeamMemberInfoList::NativeConstruct()
{
	Super::NativeConstruct(); 

	MemberWidgets.Empty(); 
	MemberWidgets.Add(TeamMember1); 
	MemberWidgets.Add(TeamMember2); 
	MemberWidgets.Add(TeamMember3); 
	MemberWidgets.Add(TeamMember4); 
	MemberWidgets.Add(TeamMember5); 
}

void UTeamMemberInfoList::SetTeamMemberInfoList()
{
	check(WidgetController); 

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		if (AOWGameState* OWGameState = GetWorld()->GetGameState<AOWGameState>())
		{
			AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(OverlayWidgetController->PlayerState);
			const int32 MyTeam = OWPlayerState->GetTeamId();

			for (APlayerState* PlayerState : OWGameState->PlayerArray)
			{
				if (AOWPlayerState* MemberPlayerState = Cast<AOWPlayerState>(PlayerState))
				{
					if (MemberPlayerState->GetTeamId() == MyTeam)
					{
						TeamPlayerStates.Add(MemberPlayerState); 
						MemberPlayerState->OnHeroNameChangedDelegate.AddUObject(this, &UTeamMemberInfoList::HandleHeroNameChanged); 
						HandleHeroNameChanged(MemberPlayerState, MemberPlayerState->GetHeroName()); 
					}
				}
			}
		}
	}
}

void UTeamMemberInfoList::HandleHeroNameChanged(AOWPlayerState* OWPlayerState, EHeroName NewHeroName)
{
	int32 TeamMemberIndex = TeamPlayerStates.IndexOfByKey(OWPlayerState); 
	if (TeamMemberIndex == INDEX_NONE || !MemberWidgets.IsValidIndex(TeamMemberIndex))
	{
		return;
	}

	UTeamMemberInfo* TeamMemberInfo = MemberWidgets[TeamMemberIndex];
	TeamMemberInfo->HandleHeroNameChanged(OWPlayerState, NewHeroName);
}




