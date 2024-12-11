// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalManager.h"
#include "HttpModule.h"
#include "Data/API/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "GameFramework/PlayerState.h"

void UPortalManager::JoinGameSession()
{
	BroadcastJoinGameSessionMessage.Broadcast(TEXT("Searching for Game Session..."), false); 

	check(APIData); 
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); 
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::FindOrCreateGameSession_Response); 
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::FindOrCreateGameSession); 
	Request->SetURL(APIUrl); 
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json")); 
	Request->ProcessRequest(); 
}

void UPortalManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true); 
	}

	TSharedPtr<FJsonObject> JsonObject; 
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString()); 
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		}
		FDSGameSession GameSession; 
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSession); 

		const FString GameSessionStatus = GameSession.Status;
		const FString GameSessionId = GameSession.GameSessionId; 
		HandleGameSessionStatus(GameSessionStatus, GameSessionId);
	}
}

FString UPortalManager::GetUniquePlayerId() const
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		APlayerState* LocalPlayerState = LocalPlayerController->GetPlayerState<APlayerState>();
		if (IsValid(LocalPlayerState) && LocalPlayerState->GetUniqueId().IsValid())
		{
			return TEXT("Player_") + FString::FromInt(LocalPlayerState->GetUniqueID());
		}
	}
	return FString(); 
}

void UPortalManager::HandleGameSessionStatus(const FString& Status, const FString& SessionId)
{
	if (Status.Equals(TEXT("ACTIVE")))
	{
		BroadcastJoinGameSessionMessage.Broadcast(TEXT("Found Active Game Session. Creating a Player Session..."), false);
		TryCreatePlayerSession(GetUniquePlayerId(), SessionId); 
	}
	else if (Status.Equals(TEXT("ACTIVATING")))
	{
		FTimerDelegate CreateSessionDelegate; 
		CreateSessionDelegate.BindUObject(this, &ThisClass::JoinGameSession); 
		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			LocalPlayerController->GetWorldTimerManager().SetTimer(CreateSessionTimer, CreateSessionDelegate, 0.5f, false); 
		}
	}
	else
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true); 
	}
}

void UPortalManager::TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionId)
{
	check(APIData); 
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); 
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::CreatePlayerSession_Response); 
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::CreatePlayerSession); 
	Request->SetURL(APIUrl); 
	Request->SetVerb(TEXT("POST")); 
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json")); 

	TMap<FString, FString> Params = {
		{ TEXT("playerId"), PlayerId}, 
		{ TEXT("gameSessionId"), GameSessionId}
	};
	const FString Content = SerializeJsonContent(Params); 

	Request->SetContentAsString(Content); 
	Request->ProcessRequest();
}

void UPortalManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

}