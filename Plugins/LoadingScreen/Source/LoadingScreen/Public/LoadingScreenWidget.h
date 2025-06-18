// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOADINGSCREEN_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> DefaultLoadingScreenWidgetClass; 

protected:
	virtual void NativeConstruct() override; 
	
private:
	UFUNCTION()
	void OnLoadingScreenWidgetChanged(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY()
	TSubclassOf<UUserWidget> ContentWidgetClass; 
};
