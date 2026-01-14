// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "ReinhardtBarrierWidget.generated.h"

class UTextBlock;
class UImage; 
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UReinhardtBarrierWidget : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_BarrierField;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_BarrierField;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthUnit = 60.f;

protected:
	virtual void NativeConstruct() override; 

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void UpdateBarrierHealth(float NewHealthValue); 

	UFUNCTION()
	void UpdateBarrierMaxHealth(float NewMaxHealthValue);

	TWeakObjectPtr<UAbilitySystemComponent> BarrierASC;

	TObjectPtr<UMaterialInstanceDynamic> MID_BarrierField;

	UPROPERTY()
	float SavedMaxHealth = -1.f;
};
