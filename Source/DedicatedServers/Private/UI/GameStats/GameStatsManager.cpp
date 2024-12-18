// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStats/GameStatsManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "JsonObjectConverter.h"
#include "HttpModule.h"
#include "Data/API/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"

void UGameStatsManager::RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput)
{
	// If you want a JsonObject
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(); 
	FJsonObjectConverter::UStructToJsonObject(FDSRecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonObject.ToSharedRef()); 

	FString JsonString;  
	FJsonObjectConverter::UStructToJsonObjectString(FDSRecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonString); 

	GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Red, JsonString); 

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); 

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::RecordMatchStats); 
	Request->SetURL(APIUrl); 
	Request->SetVerb("POST"); 
	Request->SetHeader("Content-Type", "application/json"); 
	Request->SetContentAsString(JsonString); 
	Request->ProcessRequest(); 
}


