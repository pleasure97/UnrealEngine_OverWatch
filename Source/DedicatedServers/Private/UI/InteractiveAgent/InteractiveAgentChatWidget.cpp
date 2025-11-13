// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractiveAgent/InteractiveAgentChatWidget.h"
#include "UI/InteractiveAgent/InteractiveAgentManager.h"
#include "Game/InteractiveAgentSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/ScrollBox.h"
#include "UI/InteractiveAgent/InteractWidget.h"
#include "UI/InteractiveAgent/MessageItemWidget.h"
#include "Blueprint/WidgetTree.h"

void UInteractiveAgentChatWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Create Interactive Agent Manager 
	InteractiveAgentManager = NewObject<UInteractiveAgentManager>(this, InteractiveAgentManagerClass); 

	// Get Interactive Agent Subsystem and Bind Interactive Chat History Updated Delegate
	if (InteractiveAgentManager)
	{
		if (UInteractiveAgentSubsystem* InteractiveAgentSubsystem = InteractiveAgentManager->GetInteractiveAgentSubsystem())
		{
			InteractiveAgentSubsystem->OnInteractiveChatHistoryUpdated.AddDynamic(
				this, &UInteractiveAgentChatWidget::OnInteractiveChatHistoryUpdated);
		}
	}

	// Bind Button Clicked Delegate
	if (Button_InteractWithInteractiveAgent)
	{
		Button_InteractWithInteractiveAgent->OnClicked.AddDynamic(this, &UInteractiveAgentChatWidget::OnEnterMessage); 
	}
}

FReply UInteractiveAgentChatWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// KeyBoard Enter - Send User Prompt to LLM API and Show User Prompt in the Chat Window
	if (InKeyEvent.GetKey() == EKeys::Enter && !InKeyEvent.IsShiftDown())
	{
		OnEnterMessage();

		return FReply::Handled().SetUserFocus(MultiLineEditableTextBox_UserPrompt->TakeWidget());
	}

	// KeyBoard Escape - Collapse Chat Window
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		EndChat();

		return FReply::Handled().SetUserFocus(MultiLineEditableTextBox_UserPrompt->TakeWidget());
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UInteractiveAgentChatWidget::OnEnterMessage()
{
	if (MultiLineEditableTextBox_UserPrompt && ScrollBox_SystemResponse)
	{
		// Get User Prompt String with Front and Back Trimmed
		FString UserPrompt = MultiLineEditableTextBox_UserPrompt->GetText().ToString().TrimStartAndEnd();
		if (!UserPrompt.IsEmpty())
		{
			SendPromptToLLM(UserPrompt);

			AddChatMessage(UserPrompt, true);

			MultiLineEditableTextBox_UserPrompt->SetText(FText::GetEmpty());
		}
	}
}

void UInteractiveAgentChatWidget::ActivateInteraction()
{
	SetVisibility(ESlateVisibility::Visible); 

	if (VerticalBox_InteractiveAgentChat)
	{
		VerticalBox_InteractiveAgentChat->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInteractiveAgentChatWidget::StartInteraction()
{
	if (WBP_InteractionWidget)
	{
		WBP_InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (VerticalBox_InteractiveAgentChat)
	{
		VerticalBox_InteractiveAgentChat->SetVisibility(ESlateVisibility::Visible);
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), MultiLineEditableTextBox_UserPrompt);
	}
}

void UInteractiveAgentChatWidget::EndChat()
{
	SetVisibility(ESlateVisibility::Collapsed);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetOwningPlayer());
}

void UInteractiveAgentChatWidget::OnInteractiveChatHistoryUpdated()
{
	// AddChatMessage()
}

void UInteractiveAgentChatWidget::SendPromptToLLM(const FString& Prompt)
{
	if (UInteractiveAgentSubsystem* InteractiveAgentSubsystem = InteractiveAgentManager->GetInteractiveAgentSubsystem())
	{

	}
}

void UInteractiveAgentChatWidget::AddChatMessage(const FString& Message, bool bIsUserMessage)
{
	if (ScrollBox_SystemResponse && MessageItemWidgetClass)
	{
		// TODO - May need to Add Widget Name 
		UMessageItemWidget* NewMessageItem = WidgetTree->ConstructWidget<UMessageItemWidget>(MessageItemWidgetClass); 
		if (NewMessageItem)
		{
			NewMessageItem->SetMessageContent(Message, bIsUserMessage); 
			ScrollBox_SystemResponse->AddChild(NewMessageItem); 
			// ScrollBox_SystemResponse->ScrollToEnd(); 
		}
	}
}
