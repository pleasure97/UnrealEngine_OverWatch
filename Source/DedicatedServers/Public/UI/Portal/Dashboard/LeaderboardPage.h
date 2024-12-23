// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardPage.generated.h"

class UScrollBox; 
class UTextBlock; 
class ULeaderboardCard; 
struct FDSLeaderboardItem; 
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ULeaderboardPage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void PopulateLeaderboard(TArray<FDSLeaderboardItem>& Leaderboard); 

	UFUNCTION()
	void SetStatusMessage(const FString& StatusMessage, bool bShouldResetWidgets);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Leaderboard; 
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage; 

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULeaderboardCard> LeaderboardCardClass; 
	
private:
	void CalculateLeaderboardPlaces(TArray<FDSLeaderboardItem>& OutLeaderboard); 
};
