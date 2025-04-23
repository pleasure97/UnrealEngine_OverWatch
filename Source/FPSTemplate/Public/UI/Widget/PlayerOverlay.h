// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "PlayerOverlay.generated.h"

class UHealthBarPool; 
class UPlayerHealthStatus; 
class UPlayerSkills; 
class UPlayerWeaponStatus; 
class UUltimateGauge; 
class UHitIndicatorPool; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPlayerOverlay : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHealthBarPool> WBP_PlayerHealthBarPool; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPlayerHealthStatus> WBP_PlayerHealthStatus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPlayerSkills> WBP_PlayerSkills;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPlayerWeaponStatus> WBP_PlayerWeaponStatus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUltimateGauge> WBP_UltimateGauge;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHitIndicatorPool> WBP_HitIndicatorPool; 

	virtual void SetChildWidgetControllers() override; 
};
