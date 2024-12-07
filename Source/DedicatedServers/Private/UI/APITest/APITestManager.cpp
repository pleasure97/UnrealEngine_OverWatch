// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APITest/APITestManager.h"
#include "HttpModule.h"
#include "Data/API/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UAPITestManager::ListFleetsButtonClicked()
{
	check(APIData); 

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); 

	Request->OnProcessRequestComplete().BindUObject(this, &UAPITestManager::ListFleets_Response); 

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::ListFleets); 

	Request->SetURL(APIUrl); 
	Request->SetVerb(TEXT("GET")); 
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->ProcessRequest(); 
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "List Fleets Request Made");
}

void UAPITestManager::ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "List Fleets Response Received");

	TSharedPtr<FJsonObject> JsonObject; 
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (JsonObject->HasField(TEXT("errorType")) || JsonObject->HasField(TEXT("errorMessage")))
		{
			FString ErrorType = JsonObject->HasField(TEXT("errorType")) ? JsonObject->GetStringField(TEXT("errorType")) : TEXT("Unknown Error Type"); 
			FString ErrorMessage = JsonObject->HasField(TEXT("errorMessage")) ? JsonObject->GetStringField(TEXT("errorMessage")) : TEXT("Unknown Error Message");

			UE_LOG(LogDedicatedServers, Error, TEXT("Error Type : %s"), *ErrorType); 
			UE_LOG(LogDedicatedServers, Error, TEXT("Error Message : %s"), *ErrorMessage); 

			return; 
		}

		if (JsonObject->HasField(TEXT("$fault")))
		{
			FString ErrorType = JsonObject->HasField(TEXT("name")) ? JsonObject->GetStringField(TEXT("name")) : TEXT("Unknown Error");
			UE_LOG(LogDedicatedServers, Error, TEXT("Error Type : %s"), *ErrorType);

			return; 
		}

		if (JsonObject->HasField(TEXT("$metadata")))
		{
			TSharedPtr<FJsonObject> MetaDataJsonObject = JsonObject->GetObjectField(TEXT("$metadata")); 
			FDSMetaData DSMetaData; 
			FJsonObjectConverter::JsonObjectToUStruct(MetaDataJsonObject.ToSharedRef(), &DSMetaData); 

			DSMetaData.Dump(); 
		}

		FDSListFleetsResponse ListFleetsResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &ListFleetsResponse); 
		ListFleetsResponse.Dump(); 
	}
}
