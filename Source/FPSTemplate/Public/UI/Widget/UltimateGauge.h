// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "UltimateGauge.generated.h"

class UImage; 
class UBorder; 
class UTextBlock; 
class UMaterialInstanceDynamic; 



/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UUltimateGauge : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_UltimateGauge; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Percent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_UltimateIcon; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_UltimateIcon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_UltimateRay; 

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> UltimateGaugeMID;

	/* Settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor OrangeColor = FLinearColor(0.9568f, 0.2067f, 0.044f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor SkyColor = FLinearColor(0.133824f, 0.988235f, 0.983409f, 1.f);

	UFUNCTION()
	void ReceiveAbilityInfo(const FOWAbilityInfo& Info);

	UFUNCTION()
	void SetMaxUltimateGauge(float NewValue); 

	UFUNCTION()
	void UpdateUltimateGauge(float NewValue); 

	float Percent = 0.f; 

	float CurrentUltimateGauge = 0.f; 

	float MaxUltimateGauge = 0.f; 

	bool bAlreadyUpdated = false;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 
};
