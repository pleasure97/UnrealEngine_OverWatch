// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonUISubsystem.generated.h"

class UPrimaryLayoutWidget; 
struct FGameplayTag;
class UActivatableBaseWidget;
class UOWCommonButtonBase;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush, 
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegate, UOWCommonButtonBase*, BroadcastingButton, FText, DescriptionText);
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UCommonUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UCommonUISubsystem* Get(const UObject* WorldContextObject); 

	/* USubsystem Interface Begins */
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	/* USubsystem Interface Ends */

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UPrimaryLayoutWidget* InCreatedWidget);

	void PushSofWidgetToStackAsync(
		const FGameplayTag& InWidgetStackTag, 
		TSoftClassPtr<UActivatableBaseWidget> InSoftWidgetClass, 
		TFunction<void(EAsyncPushWidgetState, UActivatableBaseWidget*)> AsyncPushStateCallback);

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;

private:
	UPROPERTY(Transient)
	UPrimaryLayoutWidget* CreatedPrimaryLayoutWidget;
};
