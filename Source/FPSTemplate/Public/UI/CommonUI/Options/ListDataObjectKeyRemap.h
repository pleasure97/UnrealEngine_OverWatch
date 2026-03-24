// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/Options/ListDataObjectBase.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObjectKeyRemap.generated.h"

class UEnhancedInputUserSettings;
class UEnhancedPlayerMappableKeyProfile;
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UListDataObjectKeyRemap : public UListDataObjectBase
{
	GENERATED_BODY()
	
public:
	void InitKeyRemapData(
		UEnhancedInputUserSettings* InOwningInputUserSettings, 
		UEnhancedPlayerMappableKeyProfile* InKeyProfile, 
		ECommonInputType InDesiredInputKeyType, 
		const FPlayerKeyMapping& InOwningPlayerKeyMapping);

	FText GetCategoryName() const { return CategoryName; }

	void SetCategoryName(FText InCategoryName);

private:
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningInputUserSettings;

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;

	ECommonInputType CachedDesiredInputKeyType;

	FName CachedOwningMappingName;

	EPlayerMappableKeySlot CachedOwningMappableKeySlot;

	FText CategoryName;
};
