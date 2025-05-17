// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGamePage.generated.h"

class UButton; 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UEndGamePage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_CancelEndGame; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_EndGame; 

	UFUNCTION()
	void OnCancelEndGameButtonClicked(); 

	UFUNCTION()
	void OnEndGameButtonClicked(); 
	
protected:
	virtual void NativeConstruct() override; 
};
