// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OWHUD.generated.h"

class UOWUserWidget; 
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
	TObjectPtr<UOWUserWidget> PlayerOverlay; 

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams); 

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS); 

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOWUserWidget> OverlayWidgetClass; 
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController; 

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass; 

	bool bInitialized = false; 
};
