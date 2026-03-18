// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ActivatableBaseWidget.generated.h"

class AOWPlayerController; 

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class FPSTEMPLATE_API UActivatableBaseWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure)
	AOWPlayerController* GetOwningOWPlayerController(); 

private:
	TWeakObjectPtr<AOWPlayerController> CachedOwningPlayerController;
};
