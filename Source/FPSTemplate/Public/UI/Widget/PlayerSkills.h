// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "PlayerSkills.generated.h"

class UPlayerSkill; 
class UHorizontalBox; 
class UBorder; 
struct FOWAbilityInfo; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPlayerSkills : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerSkill> PlayerSkillClass; 
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_PlayerSkills; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_LShift;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_E;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_F;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_LMB; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, TObjectPtr<UBorder>> TagsToBorders; 

	UFUNCTION()
	void SetChildToBorder(const FOWAbilityInfo& Info); 

protected:
	virtual void NativePreConstruct() override; 
	virtual void NativeConstruct() override; 

};
