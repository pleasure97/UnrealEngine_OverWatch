// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenSubsystem.generated.h"

class UUserWidget; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadingScreenWidgetChangedDelegate, TSubclassOf<UUserWidget>, NewWidgetClass);

/**
 * 
 */
UCLASS()
class LOADINGSCREEN_API ULoadingScreenSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	ULoadingScreenSubsystem(); 

	UFUNCTION(BlueprintCallable)
	void SetLoadingScreenWidget(TSubclassOf<UUserWidget> NewWidgetClass); 

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetLoadingScreenWidget() const; 

private:
	UPROPERTY(BlueprintAssignable, meta=(AllowPrivateAccess))
	FLoadingScreenWidgetChangedDelegate OnLoadingScreenWidgetChanged; 

	UPROPERTY()
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass; 
};
