// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "Interfaces/IHttpRequest.h"
#include "GameStatsManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRetrieveStatsResponseReceived, const FDSRetrieveMatchStatsResponse&, RetrieveMatchStatsResponse); 


struct FDSRecordMatchStatsInput; 
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UGameStatsManager : public UHTTPRequestManager
{
	GENERATED_BODY()
	
public:
	void RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput); 
	void RetrieveMatchStats(); 

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessage RetrieveMatchStatsStatusMessage; 
	
	UPROPERTY()
	FOnRetrieveStatsResponseReceived OnRetrieveMatchStatsResponseReceived;

private:
	void RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); 
	void RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); 
};
