// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "HeroRoster.generated.h"

class UTeamMemberInfoList; 
class UHeroSelectionList; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHeroRoster : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTeamMemberInfoList> WBP_TeamMemberInfoList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHeroSelectionList> WBP_HeroSelectionList;
};
