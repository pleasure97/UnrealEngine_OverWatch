// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/ListEntries/ListEntryBaseWidget.h"
#include "ListEntryKeyRemapWidget.generated.h"

class UOWCommonButtonBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class FPSTEMPLATE_API UListEntryKeyRemapWidget : public UListEntryBaseWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess="true"))
	UOWCommonButtonBase* CommonButton_KeyboardRemapKey;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UOWCommonButtonBase* CommonButton_SecondaryRemapKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UOWCommonButtonBase* CommonButton_GamepadRemapKey;
};
