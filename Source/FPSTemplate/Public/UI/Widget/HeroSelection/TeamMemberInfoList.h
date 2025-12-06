// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "TeamMemberInfoList.generated.h"

class UTeamMemberInfo; 
class UHorizontalBox; 
class AOWPlayerState; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UTeamMemberInfoList : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumTeamMembers = 5; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UTeamMemberInfo> TeamMemberInfoClass; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMargin IntervalBetweenTeamMemberInfo = FMargin(0.f, 0.f, 62.5f, 0.f); 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_TeamMemberInfoList; 

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	void UpdateTeamMember(); 

	UFUNCTION()
	void HandleNewPlayerState(APlayerState* PlayerState);

	UFUNCTION()
	void HandleRemovedPlayerState(APlayerState* PlayerState); 

	UFUNCTION()
	void OnTeamMemberHeroChanged(AOWPlayerState* OWPlayerState, EHeroName HeroName);

	UFUNCTION()
	void OnTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID);

	UFUNCTION()
	void OnOwningPlayerStateChanged(APlayerState* NewPlayerState);

	void UpdateTeamMemberWidget(TMap<AOWPlayerState*, UTeamMemberInfo*>& TeamMemberInfoMap);

	int32 MyTeamID = -1; 

	TMap<AOWPlayerState*, UTeamMemberInfo*> Team1MemberInfoMap; 
	TMap<AOWPlayerState*, UTeamMemberInfo*> Team2MemberInfoMap; 
};
