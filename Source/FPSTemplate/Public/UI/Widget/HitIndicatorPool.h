// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "Blueprint/UserWidgetPool.h"
#include "HitIndicatorPool.generated.h"

class UOverlay; 
class UHitIndicator; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHitIndicatorPool : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHitIndicator> HitIndicatorClass; 

	UFUNCTION()
	void ProcessDamageReceived(AActor* DamageCauser, AActor* OwnerActor, float Damage); 

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 
};
