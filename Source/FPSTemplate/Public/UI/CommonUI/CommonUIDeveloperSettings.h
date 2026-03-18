// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "CommonUIDeveloperSettings.generated.h"

class UActivatableBaseWidget;
/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Common UI Settings"))
class FPSTEMPLATE_API UCommonUIDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "CommonUI.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UActivatableBaseWidget>> CommonUIWidgetMap;
};
