// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/TeamMemberInfoList.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Player/OWPlayerState.h"
#include "Game/OWGameState.h"
#include "UI/Widget/HeroSelection/TeamMemberInfo.h"
#include "Team/OWTeamSubsystem.h"

void UTeamMemberInfoList::NativeConstruct()
{
	Super::NativeConstruct();

	// Create Team Member Info Widget Dynamically as many as the Number of Team Members
	for (int32 i = 0; i < NumTeamMembers; ++i)
	{
		UTeamMemberInfo* NewTeamMemberInfo = CreateWidget<UTeamMemberInfo>(this, TeamMemberInfoClass);
		if (HorizontalBox_TeamMemberInfoList && NewTeamMemberInfo)
		{
			// Add Team Member Info Widget as the child of Horizontal Box 
			UHorizontalBoxSlot* NewHorizontalBoxSlot = HorizontalBox_TeamMemberInfoList->AddChildToHorizontalBox(NewTeamMemberInfo);
			// Set Padding between Team Member Info Widgets 
			if (i != NumTeamMembers - 1)
			{
				NewHorizontalBoxSlot->SetPadding(IntervalBetweenTeamMemberInfo);
			}
			// Assign the generated team member info widget to Team1 and Team2 Member Info Arrays
			Team1MemberInfoArray.Emplace(nullptr, NewTeamMemberInfo);
			Team2MemberInfoArray.Emplace(nullptr, NewTeamMemberInfo);
		}
	}

	// Get Game State 
	UWorld* World = GetOwningPlayer() ? GetOwningPlayer()->GetWorld() : nullptr; 
	if (AOWGameState* OWGameState = Cast<AOWGameState>(World->GetGameState()))
	{
		// Call Callback Function when Player State is Added or Removed 
		OWGameState->OnPlayerStateAdded.AddDynamic(this, &UTeamMemberInfoList::HandleNewPlayerState); 
		OWGameState->OnPlayerStateRemoved.AddDynamic(this, &UTeamMemberInfoList::HandleRemovedPlayerState); 
		// Get Player Array of Game State and Take Care of Every Player State
		for (APlayerState* PlayerState : OWGameState->PlayerArray)
		{
			HandleNewPlayerState(PlayerState);
		}
	}
}

void UTeamMemberInfoList::HandleNewPlayerState(APlayerState* PlayerState)
{
	// Bind All Player's Team Changed Delegate and Hero Name Changed Delegate 
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState))
	{
		OWPlayerState->GetTeamChangedDelegate().AddDynamic(this, &UTeamMemberInfoList::OnClientTeamChanged);
		OWPlayerState->OnHeroNameChangedDelegate.AddUObject(this, &UTeamMemberInfoList::OnTeamMemberHeroChanged);
		// Call if Team ID has already been Assigned to the PlayerState
		if ((OWPlayerState->GetTeamId() == 1) || (OWPlayerState->GetTeamId() == 2))
		{
			OnClientTeamChanged(OWPlayerState, -1, OWPlayerState->GetTeamId());
		}
	}
}

void UTeamMemberInfoList::HandleRemovedPlayerState(APlayerState* PlayerState)
{
	// Remove All Player's Team Changed Delegate and Hero Name Changed Delegate 
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState))
	{
		OWPlayerState->GetTeamChangedDelegate().RemoveAll(this); 
		OWPlayerState->OnHeroNameChangedDelegate.RemoveAll(this); 

		// TODO - Change UI, e.g., Portrait, Nickname, ...
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
	Team1MemberInfoArray.Empty();
	Team2MemberInfoArray.Empty(); 

	Super::NativeDestruct();
}

void UTeamMemberInfoList::OnClientTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID)
{
	// New Team ID should be only 1 or 2 
	if ((NewTeamID != 1) && (NewTeamID != 2))
	{
		UE_LOG(LogTemp, Error, TEXT("Team ID is not 0 or 1 in UTeamMemberInfoList::OnTeamChanged()")); 
		return; 
	}

	// Cast to Custom Player State 
	if (AOWPlayerState* ClientPlayerState = Cast<AOWPlayerState>(ObjectChangingTeam))
	{
		// Get Owning Player State and Cast it to Custom Player State 
		AOWPlayerState* OwnerPlayerState = Cast<AOWPlayerState>(GetOwningPlayerState()); 
		// Check if Team Changed Player State is Mine 
		if (ClientPlayerState == OwnerPlayerState)
		{
			MyTeamID = ClientPlayerState->GetTeamId(); 
			if (MyTeamID == 1)
			{
				// Initialize Ally Team Member Info 
				for (TPair<AOWPlayerState*, UTeamMemberInfo*>& Team1MemberInfo : Team1MemberInfoArray)
				{
					// Find Empty Team Member Info Element and Initialize Team Member Info in it 
					if (Team1MemberInfo.Value->GetOWPlayerState() == nullptr)
					{
						Team1MemberInfo.Key = ClientPlayerState;
						Team1MemberInfo.Value->RefreshTeamMemberInfo(ClientPlayerState);
						break;
					}
				}

				// Remove Enemy Team Member Infos
				for (TPair<AOWPlayerState*, UTeamMemberInfo*>& Team2MemberInfo : Team2MemberInfoArray)
				{
					// Remove Bindings of Enemies' Delegates 
					if (Team2MemberInfo.Key != nullptr)
					{
						Team2MemberInfo.Key->GetTeamChangedDelegate().RemoveAll(this);
						Team2MemberInfo.Key->OnHeroNameChangedDelegate.RemoveAll(this);
					}
				}
				// Clear Enemy Member Info Array 
				Team2MemberInfoArray.Empty(); 
			}
			else if (MyTeamID == 2)
			{
				// Find Empty Team Member Info Element and Initialize Team Member Info in it 
				for (TPair<AOWPlayerState*, UTeamMemberInfo*>& Team2MemberInfo : Team2MemberInfoArray)
				{
					if (Team2MemberInfo.Value->GetOWPlayerState() == nullptr)
					{
						Team2MemberInfo.Key = ClientPlayerState;
						Team2MemberInfo.Value->RefreshTeamMemberInfo(ClientPlayerState);
						break;
					}
				}

				// Remove Enemy Team Member Infos
				for (TPair<AOWPlayerState*, UTeamMemberInfo*>& Team1MemberInfo : Team1MemberInfoArray)
				{
					// Remove Bindings of Enemies' Delegates 
					if (Team1MemberInfo.Key != nullptr)
					{
						Team1MemberInfo.Key->GetTeamChangedDelegate().RemoveAll(this);
						Team1MemberInfo.Key->OnHeroNameChangedDelegate.RemoveAll(this);
					}
				}
				// Clear Enemy Member Info Array 
				Team1MemberInfoArray.Empty(); 
			}
			else
			{
				return; 
			}
		}
		// Initialize Ally Team Member Info 
		else
		{
			if (NewTeamID == 1)
			{
				for (TPair<AOWPlayerState*, UTeamMemberInfo*>& Team1MemberInfo : Team1MemberInfoArray)
				{
					if (Team1MemberInfo.Value->GetOWPlayerState() == nullptr)
					{
						Team1MemberInfo.Key = ClientPlayerState;
						Team1MemberInfo.Value->RefreshTeamMemberInfo(ClientPlayerState);
						break;
					}
				}
			}
			else if (NewTeamID == 2)
			{
				for (TPair<AOWPlayerState*, UTeamMemberInfo*>& Team2MemberInfo : Team2MemberInfoArray)
				{
					if (Team2MemberInfo.Value->GetOWPlayerState() == nullptr)
					{
						Team2MemberInfo.Key = ClientPlayerState;
						Team2MemberInfo.Value->RefreshTeamMemberInfo(ClientPlayerState);
						break;
					}
				}
			}
			else
			{
				return; 
			}
		}
	}

	// If Team1 Member Info Array is not Removed, Sort Team1 Member Info Array 
	if (!Team1MemberInfoArray.IsEmpty())
	{
		SortTeamMemberInfoArray(Team1MemberInfoArray);
	}
	
	// If Team2 Member Info Array is not Removed, Sort Team2 Member Info Array 
	if (!Team2MemberInfoArray.IsEmpty())
	{
		SortTeamMemberInfoArray(Team2MemberInfoArray); 
	}
}

void UTeamMemberInfoList::SortTeamMemberInfoArray(TArray<TPair<AOWPlayerState*, UTeamMemberInfo*>>& TeamMemberInfoArray)
{
	// Sort the array so that the key that is not nullptr is at the front
	TeamMemberInfoArray.Sort(
		[](const TPair<AOWPlayerState*, UTeamMemberInfo*>& A, const TPair<AOWPlayerState*, UTeamMemberInfo*>& B)
		{
			return (A.Key != nullptr) && (B.Key == nullptr);
		}
	);

	if (HorizontalBox_TeamMemberInfoList)
	{
		// Empty Horizontal Box 
		HorizontalBox_TeamMemberInfoList->ClearChildren();
		// Iterate Team Member Info Array 
		for (int32 i = 0; i < TeamMemberInfoArray.Num(); ++i) 
		{
			// If Player State of Team Member Info Array is nullptr, Set Nickname Visibility to be Collapsed 
			TeamMemberInfoArray[i].Value->RefreshTeamMemberInfo(TeamMemberInfoArray[i].Key);
			UHorizontalBoxSlot* SortedHorizontalBoxSlot = HorizontalBox_TeamMemberInfoList->AddChildToHorizontalBox(TeamMemberInfoArray[i].Value);
			if (i != (TeamMemberInfoArray.Num() - 1))
			{
				SortedHorizontalBoxSlot->SetPadding(IntervalBetweenTeamMemberInfo);
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
	if (MyTeamID == 1)
	{
		for (TPair<AOWPlayerState*, UTeamMemberInfo*>& Team1MemberInfo : Team1MemberInfoArray)
		{
			// Find Ally Player State and Team Member Info 
			if (OWPlayerState == Team1MemberInfo.Key)
			{
				// Change Team Member Info 
				Team1MemberInfo.Value->HandleHeroNameChanged(OWPlayerState, OWPlayerState->GetHeroName()); 
			}
		}
	}
	else if (MyTeamID == 2)
	{
		for (TPair<AOWPlayerState*, UTeamMemberInfo*>& Team2MemberInfo : Team2MemberInfoArray)
		{
			// Find Ally Player State and Team Member Info 
			if (OWPlayerState == Team2MemberInfo.Key)
			{
				// Change Team Member Info 
				Team2MemberInfo.Value->HandleHeroNameChanged(OWPlayerState, OWPlayerState->GetHeroName());
			}
		}
	}
	else
	{
		return;
	}
}