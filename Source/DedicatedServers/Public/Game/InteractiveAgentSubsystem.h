// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/IHttpRequest.h"
#include "InteractiveAgentSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionHistoryUpdated); 

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 InteractionUniqueID; 

	UPROPERTY(BlueprintReadWrite)
	FName SpeakerName; 

	UPROPERTY(BlueprintReadWrite)
	FString InteractiveConversation; 
};

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UInteractiveAgentSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	FOnInteractionHistoryUpdated OnInteractiveChatHistoryUpdated;

	const TArray<FInteractionData>& GetInteractiveChatHistory(); 

	UFUNCTION(BlueprintCallable)
	void StartConversation(const FName& SpeakerName, const FString& Conversation); 

private:
	void RequestInteractiveConversation(const FString& InUserPrompt); 

	void AddToInteractiveConversationHistory(const FName& SpeakerName, const FString& InInteractiveChat); 

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); 

	TArray<FInteractionData> InteractiveChatHistory; 
};
