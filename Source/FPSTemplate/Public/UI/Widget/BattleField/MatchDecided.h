// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "MatchDecided.generated.h"

class UBorder; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UMatchDecided : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	/* Animation */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> MatchDecidedAnimation; 
	
	/* UI Design Settings */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText VictoryText; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DefeatedText; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText RoundEndText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor VictoryTextColor = FLinearColor(1.f, 0.82f, 0.15f, 1.f); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor DefeatedTextColor = FLinearColor(1.f, 0.82f, 0.15f, 1.f);

	/* Widgets */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_MatchDecided;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MatchDecided;
};
