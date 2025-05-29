// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "TeamMemberInfoList.generated.h"

class UTeamMemberInfo; 
class AOWPlayerState; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UTeamMemberInfoList : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTeamMemberInfo> TeamMember1; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTeamMemberInfo> TeamMember2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTeamMemberInfo> TeamMember3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTeamMemberInfo> TeamMember4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTeamMemberInfo> TeamMember5;

	void SetTeamMemberInfoList(); 

protected:
	virtual void NativeConstruct() override; 

	UFUNCTION()
	void HandleHeroNameChanged(AOWPlayerState* OWPlayerState, EHeroName NewHeroName); 

	TArray<UTeamMemberInfo*> MemberWidgets; 

	TArray<AOWPlayerState*> TeamPlayerStates; 
};
