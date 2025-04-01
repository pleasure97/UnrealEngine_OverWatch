// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GameplayTagContainer.h"
#include "OWTeamInfoBase.generated.h"

class UOWTeamSubsystem; 

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
	FGameplayTagContainer TeamTag; 

	int32 GetTeamID() const { return TeamID; }

	virtual void BeginPlay() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void RegisterWithTeamSubsystem(UOWTeamSubsystem* Subsystem); 
	void TryRegisterWithTeamSubsystem(); 

private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamID)
	int32 TeamID = -1; 
	
	UFUNCTION()
	void OnRep_TeamID(); 

	void SetTeamID(int32 NewTeamID); 
};
