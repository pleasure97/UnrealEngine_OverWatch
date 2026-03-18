// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "PrimaryLayoutWidget.generated.h"

class UCommonActivatableWidgetContainerBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FPSTEMPLATE_API UPrimaryLayoutWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag) const;

protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(
		UPARAM(meta=(Categories="CommonUI.WidgetStack")) FGameplayTag InStackTag, 
		UCommonActivatableWidgetContainerBase * InStack);

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisterWidgetStackMap;
};
