// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "TeamMemberInfo.generated.h"

class UImage; 
class UTextBlock; 
class UThrobber; 
class AOWPlayerState; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UTeamMemberInfo : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NicknameMaxLength = 12; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroPortrait3D; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_RoleGroup; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UThrobber> Throbber_WaitingTeamMember; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Nickname;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroSelected; 

	void HandleHeroNameChanged(AOWPlayerState* OWPlayerState, EHeroName NewHeroName); 

	void InitializeTeamMemberInfo(AOWPlayerState* InOwnerPlayerState); 

	AOWPlayerState* GetOWPlayerState() const; 

protected:
	virtual void NativeConstruct() override; 

private:
	void UpdateNickName();

	bool bAlreadyChosen = true; 

	UPROPERTY()
	AOWPlayerState* OwnerPlayerState = nullptr; 
};
