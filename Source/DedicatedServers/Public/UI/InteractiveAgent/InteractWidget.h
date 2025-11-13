// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UOverlay;
class UImage;
class UBorder; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Interact; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_InteractPortrait;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_InteractKey; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_InteractKey;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PopUpAnimation;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	FTimerHandle PopUpTimerHandle;

	void PlayPopUpAnimation(); 
};
