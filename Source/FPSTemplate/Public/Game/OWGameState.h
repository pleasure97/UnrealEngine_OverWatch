// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "OWGameState.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_SelectedHeroName, BlueprintReadOnly)
	EHeroName SelectedHeroName;

	UFUNCTION()
	void SetSelectedHeroName(EHeroName NewHeroName);

	UFUNCTION()
	void OnRep_SelectedHeroName();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 
};
