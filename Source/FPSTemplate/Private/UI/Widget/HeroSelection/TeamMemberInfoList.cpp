// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/TeamMemberInfoList.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Player/OWPlayerState.h"
#include "Player/OWPlayerController.h"
#include "Game/OWGameState.h"
#include "UI/Widget/HeroSelection/TeamMemberInfo.h"
#include "Team/OWTeamSubsystem.h"

void UTeamMemberInfoList::NativeConstruct()
{
	Super::NativeConstruct();

	// Get Game State
	if (AOWGameState* OWGameState = Cast<AOWGameState>(GetWorld()->GetGameState()))
	{
		// Iterate Player Array of Game State and Bind Team Changed Delegate and Hero Name Changed Delegate 
		for (APlayerState* PlayerState : OWGameState->PlayerArray)
		{
			if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState))
			{
				OWPlayerState->GetTeamChangedDelegate().AddDynamic(this, &UTeamMemberInfoList::OnTeamChanged);
				OWPlayerState->OnHeroNameChangedDelegate.AddUObject(this, &UTeamMemberInfoList::OnTeamMemberHeroChanged);
			}
		}

		// Call Callback Function when Player State is Added or Removed 
		OWGameState->OnPlayerStateAdded.AddDynamic(this, &UTeamMemberInfoList::HandleNewPlayerState);
		OWGameState->OnPlayerStateRemoved.AddDynamic(this, &UTeamMemberInfoList::HandleRemovedPlayerState);
	}

	// Get Owning Player Controller and Bind Player State Changed Delegate
	if (AOWPlayerController* OwningPlayerController = Cast<AOWPlayerController>(GetOwningPlayer()))
	{
		OwningPlayerController->GetOnPlayerStateChangedDelegate().AddDynamic(this, &UTeamMemberInfoList::OnOwningPlayerStateChanged);
	}
}

void UTeamMemberInfoList::OnOwningPlayerStateChanged(APlayerState* NewPlayerState)
{
	// Check if Owning Player State is Valid 
	if (!IsValid(NewPlayerState))
	{
		return;
	}

	// Cast Owning Player State to Custom Player State 
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(NewPlayerState))
	{
		MyTeamID = OWPlayerState->GetTeamId();

		// If Team ID has Not been Decided Yet, Bind Team Changed Delegate of Player State
		if (MyTeamID == -1)
		{
			OWPlayerState->GetTeamChangedDelegate().AddDynamic(this, &UTeamMemberInfoList::OnTeamChanged);
		}

		UpdateTeamMember();
	}
}

void UTeamMemberInfoList::UpdateTeamMember()
{
	// Check if My Team ID is 1 or 2 
	if (MyTeamID != 1 && MyTeamID != 2)
	{
		return;
	}

	// Get Game State 
	AOWGameState* OWGameState = Cast<AOWGameState>(GetWorld()->GetGameState());
	if (!OWGameState)
	{
		return;
	}

	// Get Team Member Info Map 
	TMap<AOWPlayerState*, UTeamMemberInfo*>& TeamMemberInfoMap = (MyTeamID == 1) ? Team1MemberInfoMap : Team2MemberInfoMap;

	// Save the Number of Team Members 
	int32 OldNumTeamMembers = TeamMemberInfoMap.Num();

	// Iterate Player Array of Game State
	for (APlayerState* PlayerState : OWGameState->PlayerArray)
	{
		// Check if Player State is Cast to Custom Player State
		AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState);
		if (!OWPlayerState)
		{
			continue;
		}

		// Check if Custom Player State is Same Team 
		if (OWPlayerState->GetTeamId() != MyTeamID)
		{
			continue;
		}

		// Check if Custom Player is Already in TeamMemberInfoMap 
		if (TeamMemberInfoMap.Contains(OWPlayerState))
		{
			continue;
		}

		// Create Team Member Info Widget and Set Information related to Player State
		UTeamMemberInfo* NewTeamMemberInfo = CreateWidget<UTeamMemberInfo>(this, TeamMemberInfoClass);
		NewTeamMemberInfo->RefreshTeamMemberInfo(OWPlayerState);
		TeamMemberInfoMap.Add(TPair<AOWPlayerState*, UTeamMemberInfo*>(OWPlayerState, NewTeamMemberInfo)); 
	}

	// Check if the Number of Team Members Has Changed 
	int32 NewNumTeamMembers = TeamMemberInfoMap.Num();

	if (OldNumTeamMembers != NewNumTeamMembers)
	{
		UpdateTeamMemberWidget(TeamMemberInfoMap);
	}
}

void UTeamMemberInfoList::HandleNewPlayerState(APlayerState* PlayerState)
{
	// Bind All Player's Team Changed Delegate and Hero Name Changed Delegate 
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState))
	{
		OWPlayerState->GetTeamChangedDelegate().AddDynamic(this, &UTeamMemberInfoList::OnTeamChanged);
		OWPlayerState->OnHeroNameChangedDelegate.AddUObject(this, &UTeamMemberInfoList::OnTeamMemberHeroChanged);

		UpdateTeamMember();
	}
}

void UTeamMemberInfoList::HandleRemovedPlayerState(APlayerState* PlayerState)
{
	// Remove All Player's Team Changed Delegate and Hero Name Changed Delegate 
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState))
	{
		// Get Team Member Info Array Related to Removed Player State
		TMap<AOWPlayerState*, UTeamMemberInfo*>& RemovedTeamMemberInfoMap
			= (OWPlayerState->GetTeamId() == 1) ? Team1MemberInfoMap : Team2MemberInfoMap;

		// Iterate Team Member Info Array and Remove Player State Information
		for (TPair<AOWPlayerState*, UTeamMemberInfo*>& RemovedTeamMemberInfo : RemovedTeamMemberInfoMap)
		{
			if (RemovedTeamMemberInfo.Value->GetOWPlayerState() == OWPlayerState)
			{
				RemovedTeamMemberInfo.Key = nullptr;
				RemovedTeamMemberInfo.Value->RefreshTeamMemberInfo(nullptr);
				break;
			}
		}

		// Remove Bindings of Removed Player State Related to the Widget
		OWPlayerState->GetTeamChangedDelegate().RemoveAll(this);
		OWPlayerState->OnHeroNameChangedDelegate.RemoveAll(this);

		UpdateTeamMember();
	}
}

void UTeamMemberInfoList::NativeDestruct()
{
	// Get Owning Player State 
	if (AOWPlayerState* OwnerPlayerState = Cast<AOWPlayerState>(GetOwningPlayerState()))
	{
		// Get Player Array of Game State 
		if (AOWGameState* OWGameState = GetWorld()->GetGameState<AOWGameState>())
		{
			for (APlayerState* PlayerState : OWGameState->PlayerArray)
			{
				if (AOWPlayerState* MemberPlayerState = Cast<AOWPlayerState>(PlayerState))
				{
					// Remove Bindings of Team Member's Delegates 
					if (MemberPlayerState->GetTeamId() == OwnerPlayerState->GetTeamId())
					{
						MemberPlayerState->GetTeamChangedDelegate().RemoveAll(this);
						MemberPlayerState->OnHeroNameChangedDelegate.RemoveAll(this);
					}
				}
				// Remove Bindings of Custom Game State's Player State Delegates
				OWGameState->OnPlayerStateAdded.RemoveAll(this);
				OWGameState->OnPlayerStateRemoved.RemoveAll(this);
			}
		}
	}

	// Empty Team 1 and Team 2 Member Info Array 
	Team1MemberInfoMap.Empty();
	Team2MemberInfoMap.Empty();

	Super::NativeDestruct();
}

void UTeamMemberInfoList::OnTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID)
{
	AOWPlayerState* ClientPlayerState = Cast<AOWPlayerState>(ObjectChangingTeam);
	if (ClientPlayerState->GetTeamId() != 1 && ClientPlayerState->GetTeamId() != 2)
	{
		return;
	}

	UpdateTeamMember();
}

void UTeamMemberInfoList::UpdateTeamMemberWidget(TMap<AOWPlayerState*, UTeamMemberInfo*>& TeamMemberInfoMap)
{
	if (HorizontalBox_TeamMemberInfoList)
	{
		// Empty Horizontal Box 
		HorizontalBox_TeamMemberInfoList->ClearChildren();
		// Iterate Team Member Info Array 
		for (TPair<AOWPlayerState*, UTeamMemberInfo*>& TeamMemberInfoPair : TeamMemberInfoMap)
		{
			// If Player State of Team Member Info Array is nullptr, Set Nickname Visibility to be Collapsed 
			TeamMemberInfoPair.Value->RefreshTeamMemberInfo(TeamMemberInfoPair.Key);
			UHorizontalBoxSlot* SortedHorizontalBoxSlot = HorizontalBox_TeamMemberInfoList->AddChildToHorizontalBox(TeamMemberInfoPair.Value);
			SortedHorizontalBoxSlot->SetPadding(IntervalBetweenTeamMemberInfo);
		}

		// Create Team Member Info Widget Dynamically as many as (the Number of Total Team Members - The Number of Team Member Filled)
		for (int32 i = 0; i < NumTeamMembers - TeamMemberInfoMap.Num(); ++i)
		{
			UTeamMemberInfo* NewTeamMemberInfo = CreateWidget<UTeamMemberInfo>(this, TeamMemberInfoClass);
			if (HorizontalBox_TeamMemberInfoList && NewTeamMemberInfo)
			{
				// Add Team Member Info Widget as the child of Horizontal Box 
				UHorizontalBoxSlot* NewHorizontalBoxSlot = HorizontalBox_TeamMemberInfoList->AddChildToHorizontalBox(NewTeamMemberInfo);
				// Set Padding between Team Member Info Widgets 
				if (i != NumTeamMembers - TeamMemberInfoMap.Num() - 1)
				{
					NewHorizontalBoxSlot->SetPadding(IntervalBetweenTeamMemberInfo);
				}
			}
		}
	}
}

void UTeamMemberInfoList::OnTeamMemberHeroChanged(AOWPlayerState* OWPlayerState, EHeroName HeroName)
{
	// Check if Team Was Assigned 
	if (MyTeamID < 0)
	{
		return;
	}

	// Check whether My Team ID is 1 or 2 
	TMap<AOWPlayerState*, UTeamMemberInfo*>& TeamMemberInfoMap = (MyTeamID == 1) ? Team1MemberInfoMap : Team2MemberInfoMap;

	for (TPair<AOWPlayerState*, UTeamMemberInfo*>& TeamMemberInfo : TeamMemberInfoMap)
	{
		// Find Ally Player State and Team Member Info 
		if (OWPlayerState == TeamMemberInfo.Key)
		{
			// Change Team Member Info 
			TeamMemberInfo.Value->HandleHeroNameChanged(OWPlayerState, OWPlayerState->GetHeroName());
		}
	}
}