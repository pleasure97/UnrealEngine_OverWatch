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

	FText GetDisplayTextFromCurrentKey() const;
	FSlateBrush GetIconFromCurrentKey() const;

	ECommonInputType GetDesiredInputType() const { return CachedDesiredInputKeyType; }

	void BindNewInputKey(const FKey& InNewKey);

private:
	/* UListDataObjectBase */
	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

	FPlayerKeyMapping* GetOwningKeyMapping() const;

	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningInputUserSettings;

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;

	ECommonInputType CachedDesiredInputKeyType;

	FName CachedOwningMappingName;

	EPlayerMappableKeySlot CachedOwningMappableKeySlot;
};
