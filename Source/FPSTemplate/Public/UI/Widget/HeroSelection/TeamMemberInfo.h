// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "TeamMemberInfo.generated.h"

class UImage; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UTeamMemberInfo : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroPortrait3D; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_RoleGroup; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Nickname;
	
};
