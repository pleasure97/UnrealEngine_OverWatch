// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UI/CommonUI/CommonUIEnumTypes.h"
#include "PushConfirmScreenAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedDelegate, EConfirmScreenButtonType, ClickedButtonType);
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPushConfirmScreenAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Confirmation Screen"))
	static UPushConfirmScreenAsyncAction* PushConfirmScreen(
		const UObject* WorldContextObject,
		EConfirmScreenType ScreenType,
		FText InScreenTitle, FText InScreenMessage,
		FText InOption1Text, FText InOption2Text, FText InOption3Text
	);

	/* UBlueprintAsyncActionBase */
	virtual void Activate() override;
	/* UBlueprintAsyncActionBase Ends*/

	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld; 

	EConfirmScreenType CachedScreenType;

	FText CachedScreenTitle;

	FText CachedScreenMessage;

	FText CachedOption1Text;
	FText CachedOption2Text;
	FText CachedOption3Text;
};
