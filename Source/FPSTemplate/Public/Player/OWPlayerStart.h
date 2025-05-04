// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "GameplayTagContainer.h"
#include "OWPlayerStart.generated.h"

enum class EOWPlayerStartOccupancy
{
	Empty, 
	Partial, 
	Full
};

class AController;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Team")
	int32 TeamID = INDEX_NONE; 

	UFUNCTION(BlueprintCallable, Category = "Team")
	int32 GetTeamID() const { return TeamID;  }

	bool IsMatchingTeam(AController* Controller) const;

	EOWPlayerStartOccupancy GetLocationOccupancy(AController* const ControllerPawnToFit) const; 

	bool IsClaimed() const; 

	bool TryClaim(AController* OccupyingController); 

protected:
	void CheckUnclaimed(); 

	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr; 

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer StartPointTags; 

	UPROPERTY(EditDefaultsOnly, Category = "Player Start Claimer")
	float ExpirationCheckInterval = 1.f; 
	
	FTimerHandle ExpirationTimerHandle; 
};
