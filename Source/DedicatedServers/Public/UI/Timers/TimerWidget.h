// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/DSTypes.h"
#include "TimerWidget.generated.h"

class ADSPlayerController; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override; 
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; 
	virtual void NativeDestruct() override; 
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ECountTimerDirection TimerDirection = ECountTimerDirection::Countdown; 

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ECountTimerType TimerType = ECountTimerType::None; 

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ADSPlayerController> OwningPlayerController;

	UPROPERTY(BlueprintReadOnly)
	bool bActive = false;

	UPROPERTY(EditDefaultsOnly)
	bool bAlwaysShowMinutes = false; 

	UPROPERTY(EditDefaultsOnly)
	bool bHiddenWhenInactive = true;

	UPROPERTY(EditDefaultsOnly)
	bool bShowDecimalIfUnderTenSeconds = true; 

	UFUNCTION()
	virtual void OnTimerUpdated(float CountTime, ECountTimerDirection Direction, ECountTimerType Type);

	UFUNCTION()
	virtual void OnTimerStopped(float CountTime, ECountTimerDirection Direction, ECountTimerType Type);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Timer Started"))
	void K2_OnTimerStarted(float Time, ECountTimerDirection Direction, ECountTimerType Type);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Timer Updated"))
	void K2_OnTimerUpdated(float Time, ECountTimerDirection Direction, ECountTimerType Type);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Timer Stopped"))
	void K2_OnTimerStopped(float Time, ECountTimerDirection Direction, ECountTimerType Type);

	FString FormatTimeAsString(float TimeSeconds) const; 
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Time; 

	float InternalCountTime; 

	void TimerStarted(float InitialTime); 
	void TimerStopped(); 

	void UpdateCountTime(float TimeSeconds); 
};
