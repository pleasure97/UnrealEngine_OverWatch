// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OWHUD.generated.h"

class UPlayerOverlay; 
class UOverlayWidgetController; 
struct FWidgetControllerParams; 
class UAbilitySystemComponent; 
class UAttributeSet; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<UPlayerOverlay> PlayerOverlay; 

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams); 

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS); 

	UFUNCTION(BlueprintCallable)
	void CollapseOverlay(); 

	UFUNCTION(BlueprintCallable)
	void RemoveOverlay(); 

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerOverlay> OverlayWidgetClass; 
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController; 

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass; 

	bool bInitialized = false; 
};
