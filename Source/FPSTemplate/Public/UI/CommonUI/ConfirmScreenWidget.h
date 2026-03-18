// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/ActivatableBaseWidget.h"
#include "ConfirmScreenWidget.generated.h"

class UCommonTextBlock;
class UDynamicEntryBox;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FPSTEMPLATE_API UConfirmScreenWidget : public UActivatableBaseWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Message;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox> DynamicEntryBox_Buttons;
};
