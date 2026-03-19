// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "GameplayTagContainer.h"
#include "CommonUIDeveloperSettings.generated.h"

class UActivatableBaseWidget;
/**
 * 
 */
UCLASS(config=Game, defaultconfig, meta = (DisplayName = "Common UI Settings"))
class FPSTEMPLATE_API UCommonUIDeveloperSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()
	
public:
	UCommonUIDeveloperSettings(); 

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "CommonUI.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UActivatableBaseWidget>> CommonUIWidgetMap;
};
