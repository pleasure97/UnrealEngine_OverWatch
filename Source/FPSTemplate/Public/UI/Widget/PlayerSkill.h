// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "PlayerSkill.generated.h"

class UImage; 
class UTextBlock; 
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPlayerSkill : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SkillIcon; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_InputTag;
};
