// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractiveAgentChatWidget.generated.h"

class UVerticalBox;
class UImage;
class UTextBlock;
class UScrollBox; 
class UMultiLineEditableTextBox;
class UButton;
class UInteractiveAgentManager; 
class UInteractWidget; 
class UMessageItemWidget; 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UInteractiveAgentChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Widgets */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_InteractiveAgentChat;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_InteractiveAgentPortrait;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_InteractiveAgentName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_SystemResponse;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> MultiLineEditableTextBox_UserPrompt;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_InteractWithInteractiveAgent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractWidget> WBP_InteractionWidget; 

	/* Utils */
	UFUNCTION(BlueprintCallable)
	void ActivateInteraction(); 

	UFUNCTION(BlueprintCallable)
	void StartInteraction();

	UFUNCTION(BlueprintCallable)
	void EndChat();
	
protected:
	virtual void NativeConstruct() override; 

	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void OnEnterMessage();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInteractiveAgentManager> InteractiveAgentManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMessageItemWidget> MessageItemWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UInteractiveAgentManager> InteractiveAgentManager; 

	UFUNCTION()
	void OnInteractiveChatHistoryUpdated(); 

	UFUNCTION(BlueprintCallable)
	void SendPromptToLLM(const FString& Prompt);

	void AddChatMessage(const FString& Message, bool bIsUserMessage); 
};
