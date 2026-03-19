// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Util/CommonUIFunctionLibrary.h"
#include "UI/CommonUI/Util/CommonUIDeveloperSettings.h"

TSoftClassPtr<UActivatableBaseWidget> UCommonUIFunctionLibrary::GetCommonUISoftWidgetClassByTag(UPARAM(meta = (Categories = "CommonUI.Widget")) FGameplayTag InWidgetTag)
{
	const UCommonUIDeveloperSettings* CommonUIDeveloperSettings = GetDefault<UCommonUIDeveloperSettings>(); 

	checkf(CommonUIDeveloperSettings->CommonUIWidgetMap.Contains(InWidgetTag), TEXT("Could Not Find Corresponding Widget Under Tag %s"), *InWidgetTag.ToString()); 
	
	return CommonUIDeveloperSettings->CommonUIWidgetMap.FindRef(InWidgetTag); 
}




