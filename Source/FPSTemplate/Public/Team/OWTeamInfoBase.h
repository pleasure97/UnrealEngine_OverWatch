// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Data/GameplayTagStack.h"
#include "OWTeamInfoBase.generated.h"

class UOWTeamSubsystem; 
class UOWTeamDisplayAsset; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWTeamInfoBase : public AInfo
{
	GENERATED_BODY()
	
public:
	AOWTeamInfoBase(); 

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TeamTag; 

	int32 GetTeamID() const { return TeamID; }
	void SetTeamID(int32 NewTeamID);

	UOWTeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }
	void SetTeamDisplayAsset(UOWTeamDisplayAsset* NewDisplayAsset); 

	virtual void BeginPlay() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void RegisterWithTeamSubsystem(UOWTeamSubsystem* Subsystem); 
	void TryRegisterWithTeamSubsystem(); 

private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamID)
	int32 TeamID;
	
	UFUNCTION()
	void OnRep_TeamID(); 

	UPROPERTY(ReplicatedUsing=OnRep_TeamDisplayAsset)
	TObjectPtr<UOWTeamDisplayAsset> TeamDisplayAsset; 

	UFUNCTION()
	void OnRep_TeamDisplayAsset(); 
};