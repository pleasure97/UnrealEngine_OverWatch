// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/InteractiveAgentSubsystem.h"
#include "HttpModule.h"
#include "System/DSSystemStatics.h"
#include "Interfaces/IHttpResponse.h"

const TArray<FInteractionData>& UInteractiveAgentSubsystem::GetInteractiveChatHistory()
{
	return InteractiveChatHistory; 
}

void UInteractiveAgentSubsystem::StartConversation(const FName& SpeakerName, const FString& Conversation)
{
	UE_LOG(LogTemp, Log, TEXT("UInteractiveAgentSubsystem::StartConversation() Begins.")); 
	FString UserPrompt = SpeakerName.ToString() + ": " + Conversation; 
	RequestInteractiveConversation(UserPrompt);
}

void UInteractiveAgentSubsystem::RequestInteractiveConversation(const FString& InUserPrompt)
{
	FString Response;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); 

	const FString APIKey = UDSSystemStatics::GetAPIKeyFromDataAsset(this); 
	const FString APIUrl = UDSSystemStatics::GetAPIUrlFromDataAsset(this); 

	// Descript LLM API Call 
	Request->SetURL(APIUrl); 
	Request->SetVerb("POST"); 
	Request->SetHeader("Content-Type", "application/json"); 
	// Request->SetHeader("Authorization", "Bearer " + APIKey); 

	TSharedPtr<FJsonObject> RequestData = MakeShareable(new FJsonObject); 

	// Descript System Prompt
	FString SystemPrompt = ""; 
	const FString APIModel = UDSSystemStatics::GetAPIModelFromDataAsset(this);
	RequestData->SetStringField("user_question", InUserPrompt);
	TArray<TSharedPtr<FJsonValue>> ChatHistoryArray; 

	for (const FInteractionData& InteractiveChatData : InteractiveChatHistory)
	{
		TSharedPtr<FJsonObject> HistoryMessage = MakeShareable(new FJsonObject);

		FString Type;
		// Convert SpeakerName to "human" or "ai" which is expected from FastAPI Model
		if (InteractiveChatData.SpeakerName.ToString().Equals("Player", ESearchCase::IgnoreCase))
		{
			Type = "human";
		}
		else if (InteractiveChatData.SpeakerName.ToString().Equals("Agent", ESearchCase::IgnoreCase))
		{
			// Response to LangChain AIMessage
			Type = "ai"; 
		}

		HistoryMessage->SetStringField("type", Type);
		HistoryMessage->SetStringField("content", InteractiveChatData.InteractiveConversation);

		ChatHistoryArray.Add(MakeShareable(new FJsonValueObject(HistoryMessage)));
	}

	RequestData->SetArrayField("chat_history", ChatHistoryArray);

	// AddToInteractiveConversationHistory (응답 전에 대화 UI에 표시하기 위함)
	AddToInteractiveConversationHistory("Player", InUserPrompt);

	// Serialize JSON and Setup Request Content
	FString RequestBody;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestData.ToSharedRef(), JsonWriter);
	Request->SetContentAsString(RequestBody);

	// Bind OnResponseReceived() Callback
	Request->OnProcessRequestComplete().BindUObject(this, &UInteractiveAgentSubsystem::OnResponseReceived);
	UE_LOG(LogTemp, Log, TEXT("Processing Request in UInteractiveAgentSubsystem::RequestInteractiveConversation()"));
	Request->ProcessRequest();

	// TODO - May Need to Change Speaker Name from "Player" to Another Variable
	AddToInteractiveConversationHistory("Player", InUserPrompt); 
}

void UInteractiveAgentSubsystem::AddToInteractiveConversationHistory(const FName& InSpeakerName, const FString& InInteractiveChat)
{
	// Declare Interactive Chat Data and Assign Speaker Name and Interactive Chat Value 
	FInteractionData InteractiveChatData; 
	InteractiveChatData.SpeakerName = InSpeakerName;
	InteractiveChatData.InteractiveConversation = InInteractiveChat; 

	// Add the Above to Interactive Chat History 
	InteractiveChatHistory.Add(InteractiveChatData); 

	OnInteractiveChatHistoryUpdated.Broadcast(); 
}

void UInteractiveAgentSubsystem::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Check if Request-Response is Successful or Response is Valid 
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Call LLM API.")); 
		return; 
	}

	// Get Response Content 
	FString ResponseString = Response->GetContentAsString(); 
	UE_LOG(LogTemp, Log, TEXT("Response in OnResponseReceived() : %s"), *ResponseString); 

	// Parse JSON Response
	TSharedPtr<FJsonObject> JsonResponse;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseString); 

	// Deserialize JSON Response
	if (!FJsonSerializer::Deserialize(JsonReader, JsonResponse) || (!JsonResponse.IsValid()))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Deserialize LLM API Response.")); 
		return;
	}

	FString Content;
	if (!JsonResponse->TryGetStringField("npc_answer", Content))
	{
		UE_LOG(LogTemp, Error, TEXT("Response is missing 'npc_answer' field."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Content : %s"), *Content);

	// Add Content of Interactive Agent to Chat History 
	AddToInteractiveConversationHistory("Agent", Content);
}
