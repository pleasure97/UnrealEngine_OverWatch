// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "AsyncAction_ObserveTeam.generated.h"

class ITeamInterface; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTeamObservedAsyncDelegate, bool, bTeamSet, int32, TeamID);

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAsyncAction_ObserveTeam : public UCancellableAsyncAction
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static UAsyncAction_ObserveTeam* ObserveTeam(UObject* TeamAgent); 

	/* UBlueprintAsyncActionBase Interface */
	virtual void Activate() override; 
	virtual void SetReadyToDestroy() override;
	/* UBlueprintAsyncActionBase Interface End */

	UPROPERTY(BlueprintAssignable)
	FTeamObservedAsyncDelegate OnTeamChanged; 

private:
	UFUNCTION()
	void OnWatchedAgentChangedTeam(UObject* TeamAgent, int32 OldTeam, int32	NewTeam); 

	static UAsyncAction_ObserveTeam* InternalObserveTeamChanges(TScriptInterface<ITeamInterface> TeamActor);

	TWeakInterfacePtr<ITeamInterface> TeamInterfacePtr; 
};
