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
	Request->SetHeader("Authorization", "Bearer " + APIKey); 

	TSharedPtr<FJsonObject> RequestData = MakeShareable(new FJsonObject); 

	// Descript System Prompt
	FString SystemPrompt = ""; 
	const FString APIModel = UDSSystemStatics::GetAPIModelFromDataAsset(this);
	RequestData->SetStringField("model", APIModel);
	TArray<TSharedPtr<FJsonValue>> MessagesArray; 

	// Make System Message 
	// e.g. {"role": "system", "content": "Content, History: "}
	TSharedPtr<FJsonObject> SystemMessage = MakeShareable(new FJsonObject); 
	SystemMessage->SetStringField("role", "system"); 
	SystemMessage->SetStringField("content", SystemPrompt + "\nHistory:\n"); 

	// Add Interactive Chat History to System Prompt 
	for (FInteractionData& InteractiveChatData : InteractiveChatHistory)
	{
		SystemPrompt += InteractiveChatData.InteractiveConversation + "\n"; 
	}

	// TODO - May Need to Change Json Type according to Each LLM API 
	// Add System Message to Messages Array 
	SystemMessage->SetStringField("content", SystemPrompt); 
	MessagesArray.Add(MakeShareable(new FJsonValueObject(SystemMessage))); 

	// Make User Prompt
	TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject); 
	UserMessage->SetStringField("role", "user"); 
	UserMessage->SetStringField("content", InUserPrompt); 

	// Add User Message to Messages Array 
	MessagesArray.Add(MakeShareable(new FJsonValueObject(UserMessage))); 
	RequestData->SetArrayField("messages", MessagesArray); 

	// TODO - May Need to Change Speaker Name from "Player" to Another Variable
	AddToInteractiveConversationHistory("Player", InUserPrompt); 

	FString RequestBody; 
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestData.ToSharedRef(), JsonWriter); 
	Request->SetContentAsString(RequestBody); 

	// Bind OnResponseReceived() Callback
	Request->OnProcessRequestComplete().BindUObject(this, &UInteractiveAgentSubsystem::OnResponseReceived); 
	UE_LOG(LogTemp, Log, TEXT("Processing Request in UInteractiveAgentSubsystem::RequestInteractiveConversation()")); 
	Request->ProcessRequest(); 
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

	// Parse First Option of Choices Field
	const TArray<TSharedPtr<FJsonValue>>* ChoicesArray; 
	if (!JsonResponse->TryGetArrayField("choices", ChoicesArray) || ChoicesArray->Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Choices are Not Available in Response.")); 
		return;
	}

	// Get Choice Object from Choices Array 
	TSharedPtr<FJsonObject> ChoiceObject = (*ChoicesArray)[0]->AsObject(); 
	if (!ChoiceObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Choice Object is Invalid.")); 
		return; 
	}

	// Parse Message Field
	TSharedPtr<FJsonObject> MessageObject = ChoiceObject->GetObjectField(TEXT("message")); 
	if (!MessageObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Message Object.")); 
		return;
	}

	// Parse Message Content - Role, Content, ToneToken
	FString Role = MessageObject->GetStringField("role"); 
	FString Content = MessageObject->GetStringField("content"); 
	FString ToneToken = Content.Mid(0, Content.Find("]") + 1); 

	UE_LOG(LogTemp, Log, TEXT("Role : %s"), *Role); 
	UE_LOG(LogTemp, Log, TEXT("Content : %s"), *Content);
	UE_LOG(LogTemp, Log, TEXT("ToneToken : %s"), *ToneToken);

	// Add Content of Interactive Agent to Chat History 
	AddToInteractiveConversationHistory("Agent", Content);

	// Print Finish Reason
	FString FinishReason = ChoiceObject->GetStringField("finish_reason"); 
	UE_LOG(LogTemp, Log, TEXT("Finish Reason : %s"), *FinishReason); 
}
