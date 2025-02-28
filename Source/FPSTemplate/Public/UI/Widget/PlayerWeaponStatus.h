// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "PlayerWeaponStatus.generated.h"

class UTextBlock; 
class UImage; 
struct FOWAbilityInfo; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPlayerWeaponStatus : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumCurrentBullets; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumMaxBullets;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Slash;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Weapon; 

	UFUNCTION()
	void UpdateWeaponStatus(const FOWAbilityInfo& Info); 

	UFUNCTION()
	void UpdateNumCurrentBullets(float NewValue);

	UFUNCTION()
	void UpdateNumMaxBullets(float NewValue);

protected:
	virtual void NativeConstruct() override; 
};
