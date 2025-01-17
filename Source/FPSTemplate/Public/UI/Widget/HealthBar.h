// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar; 
class UImage; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealthBar : public UOWUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override; 

	virtual void NativeConstruct() override; 

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Style")
	TObjectPtr<UImage> Image_Background;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	FLinearColor Tint_Background;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	TObjectPtr<UImage> Image_Fill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	FLinearColor Tint_Fill;
};
