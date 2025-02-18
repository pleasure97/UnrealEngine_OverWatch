// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "PlayerHealthStatus.generated.h"

class UTextBlock; 
struct FBarInfo; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPlayerHealthStatus : public UOWUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CurrentHealth; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MaxHealth; 

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UTextBlock>> TagsToTextBlocks; 

	UPROPERTY()
	float CurrentHealth = 0.f; 
	
	UPROPERTY()
	float MaxHealth = 0.f; 

protected:
	virtual void NativePreConstruct() override; 
	virtual void NativeConstruct() override; 
	
private:
	UFUNCTION()
	void UpdatePlayerStatus(const FBarInfo& Info);
};
