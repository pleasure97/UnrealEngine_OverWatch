// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "HealthBarPool.generated.h"

class UHealthBar;
class USizeBox; 
class UHorizontalBox;
struct FAttributeDefensiveInfo; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealthBarPool : public UOWUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealthBar> HealthBarClass;

	UPROPERTY(BlueprintReadOnly)
	float HealthPerBar = 25.f;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Armor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Shield;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UHorizontalBox>> TagsToHorizontalBoxes;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root; 

private:
	UFUNCTION()
	void UpdateProgressBars(const FAttributeDefensiveInfo& Info);

	void InitializePool(const FAttributeDefensiveInfo& Info); 

	void UpdateHorizontalBoxSize(); 

	UPROPERTY()
	bool bNotInitialized = true; 

	UPROPERTY()
	int32 TotalNumBars = 0; 
};
