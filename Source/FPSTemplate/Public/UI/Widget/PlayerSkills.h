// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "PlayerSkills.generated.h"

class UPlayerSkill; 

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
	
	
};
