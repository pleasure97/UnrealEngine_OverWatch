// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DSPlayerController.h"
#include "OWPlayerController.generated.h"

class UInputMappingContext; 
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWPlayerController : public ADSPlayerController
{
	GENERATED_BODY()
	
public:
	AOWPlayerController(); 

protected:
	virtual void BeginPlay() override; 

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> OWContext; 
	
	
};
