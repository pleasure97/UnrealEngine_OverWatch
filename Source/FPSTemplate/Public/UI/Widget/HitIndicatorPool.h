// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_HitIndicatorPool; 

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHitIndicator> HitIndicatorClass; 

	UPROPERTY(EditDefaultsOnly)
	int32 NumHitIndicators = 12; 

	UFUNCTION()
	void ProcessDamageReceived(AActor* DamageCauser, AActor* OwnerActor, float Damage); 

	UFUNCTION()
	void OnHitIndicatorEnd(UHitIndicator* HitIndicator); 

protected:
	virtual void NativeConstruct() override; 

	virtual void NativeDestruct() override; 

	TArray<UHitIndicator*> IndicatorQueue; 

};
