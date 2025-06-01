// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "Interface/IndicatorWidgetInterface.h"
#include "HealthPlate.generated.h"

class UIndicatorController; 
class UTextBlock; 
class UHealthBarPool; 
class UAbilitySystemComponent;
struct FOnAttributeChangeData; 
class AOWPlayerState; 
class AActor; 
class UImage; 

namespace HealthPlateColors
{
	constexpr FLinearColor Blue(0.041667f, 0.6407f, 1.f, 1.f);
	constexpr FLinearColor Red(0.520833f, 0.005426f, 0.032740f, 1.f);
	constexpr FLinearColor White(1.f, 1.f, 1.f, 1.f); 
}

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealthPlate : public UOWUserWidget, public IIndicatorWidgetInterface
{
	GENERATED_BODY()
	
public:
	/* Indicator Widget Interface */
	void BindIndicator_Implementation(UIndicatorController* Indicator);
	void UnbindIndicator_Implementation(UIndicatorController* Indicator);
	/* Indicator Widget Interface Ends*/

	/* Callbacks */
	void OnHealthChanged(const FOnAttributeChangeData& Data); 

	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);

	void OnArmorChanged(const FOnAttributeChangeData& Data);

	void OnMaxArmorChanged(const FOnAttributeChangeData& Data);

	void OnShieldChanged(const FOnAttributeChangeData& Data);

	void OnMaxShieldChanged(const FOnAttributeChangeData& Data);

	void OnOverHealthChanged(const FOnAttributeChangeData& Data);

	/* Widgets */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHealthBarPool> WBP_HealthBarPool;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayerName; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_PlayerArrow; 

private:
	UFUNCTION()
	void OnPawnReady(UAbilitySystemComponent* ASC); 

	UFUNCTION()
	void OnDeath(AActor* DeadActor);

	UFUNCTION()
	void SetTeamVisuals(bool bTeamSet, int32 TeamID);

	void UpdateVisibility(); 

	UPROPERTY()
	TObjectPtr<AOWPlayerState> OWPlayerState; 

	UPROPERTY()
	TObjectPtr<UIndicatorController> IndicatorController; 
};
