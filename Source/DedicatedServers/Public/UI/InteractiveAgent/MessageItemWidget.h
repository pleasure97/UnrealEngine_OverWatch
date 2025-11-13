// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageItemWidget.generated.h"

class USizeBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UMessageItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_MessageItem; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MessageItem; 

	UFUNCTION(BlueprintCallable)
	void SetMessageContent(const FString& Message, bool bIsUserMessage); 

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Chat Color")
	FSlateColor UserTextColor; 

	UPROPERTY(EditDefaultsOnly, Category = "Chat Color")
	FSlateColor NPCTextColor;
};
