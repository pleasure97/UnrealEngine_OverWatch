// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Team/OWTeamPublicInfo.h"
#include "TEam/OWTeamPrivateInfo.h"
#include "TeamCreationComponent.generated.h"

class UOWTeamDisplayAsset; 
class AOWTeamPublicInfo;
class AOWTeamPrivateInfo; 
class AOWPlayerState; 

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTEMPLATE_API UTeamCreationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override; 

	bool HasAuthority() const; 
protected:
	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TMap<uint8, TObjectPtr<UOWTeamDisplayAsset>> TeamsToCreate; 

	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TSubclassOf<AOWTeamPublicInfo> PublicTeamInfoClass; 

	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TSubclassOf<AOWTeamPrivateInfo> PrivateTeamInfoClass; 

#if WITH_SERVER_CODE
	virtual void ServerCreateTeams(); 
	virtual void ServerAssignPlayersToTeams(); 
	virtual void OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer); 
	virtual void ServerChooseTeamForPlayer(AOWPlayerState* OWPlayerState); 
#endif 
private:
	void ServerCreateTeam(int32 TeamID, UOWTeamDisplayAsset* DisplayAsset); 

	int32 GetLeastPopulatedTeamID() const; 
};
