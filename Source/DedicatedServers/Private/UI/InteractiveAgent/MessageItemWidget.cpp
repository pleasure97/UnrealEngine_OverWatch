// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractiveAgent/MessageItemWidget.h"
#include "Components/TextBlock.h"

void UMessageItemWidget::SetMessageContent(const FString& Message, bool bIsUserMessage)
{
	if (TextBlock_MessageItem)
	{
		// Set Text Content 
		TextBlock_MessageItem->SetText(FText::FromString(Message)); 

		// Set Font Color 
		FSlateColor NewColor = bIsUserMessage ? UserTextColor : NPCTextColor;

		// Change Font Color of Text Block 
		TextBlock_MessageItem->SetColorAndOpacity(NewColor);
	}
}
