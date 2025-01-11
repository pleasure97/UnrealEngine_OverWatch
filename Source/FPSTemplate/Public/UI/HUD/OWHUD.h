// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OWHUD.generated.h"

class UOWUserWidget; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<UOWUserWidget> OverlayWidget; 

protected:
	virtual void BeginPlay() override; 

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOWUserWidget> OverlayWidgetClass; 
	
	
};
