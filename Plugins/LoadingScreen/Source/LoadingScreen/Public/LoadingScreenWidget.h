// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"

class UPanelWidget; 

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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> PanelWidget_Content;
	
private:
	UFUNCTION()
	void OnLoadingScreenWidgetChanged(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY()
	TSubclassOf<UUserWidget> ContentWidgetClass; 
};
