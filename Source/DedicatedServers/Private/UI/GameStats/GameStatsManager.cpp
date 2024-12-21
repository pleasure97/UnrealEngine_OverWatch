// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStats/GameStatsManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "JsonObjectConverter.h"
#include "HttpModule.h"
#include "Data/API/APIData.h"
#include "Interfaces/IHttpResponse.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "Interfaces/IHTTPRequest.h"

void UGameStatsManager::RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput)
{
	FString JsonString;  
	FJsonObjectConverter::UStructToJsonObjectString(FDSRecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonString); 

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); 

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::RecordMatchStats); 
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RecordMatchStats_Response); 
	Request->SetURL(APIUrl); 
	Request->SetVerb("POST"); 
	Request->SetHeader("Content-Type", "application/json"); 
	Request->SetContentAsString(JsonString); 
	Request->ProcessRequest(); 
}

void UGameStatsManager::RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("RecordMatchStats Failed."));
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		ContainsErrors(JsonObject);
	}
}


void UGameStatsManager::RetrieveMatchStats()
{
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem(); 

	if (!IsValid(LocalPlayerSubsystem)) return; 
	check(APIData); 

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); 
	const FString ApiUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::RetrieveMatchStats); 
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveMatchStats_Response); 
	Request->SetURL(ApiUrl); 
	Request->SetVerb("POST"); 
	Request->SetHeader("Content-Type", "application/json"); 
	
	TMap<FString, FString> Params = {
		{ TEXT("accessToken"), LocalPlayerSubsystem->GetAuthResult().AccessToken }
	}; 

	const FString Content = SerializeJsonContent(Params); 

	Request->SetContentAsString(Content); 
	Request->ProcessRequest(); 
}

void UGameStatsManager::RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnRetrieveMatchStatsResponseReceived.Broadcast(FDSRetrieveMatchStatsResponse()); 
		return;
	}

	TSharedPtr<FJsonObject> JsonObject; 
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString()); 
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnRetrieveMatchStatsResponseReceived.Broadcast(FDSRetrieveMatchStatsResponse()); 
			return;
		}
		FDSRetrieveMatchStatsResponse RetrieveMatchStatsResponse; 
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RetrieveMatchStatsResponse); 
		RetrieveMatchStatsResponse.Dump(); 

		OnRetrieveMatchStatsResponseReceived.Broadcast(RetrieveMatchStatsResponse); 

	}
}





