// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacterBase.h"
#include "OWCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWCharacter : public AOWCharacterBase
{
	GENERATED_BODY()
	
public:
	AOWCharacter(); 

	virtual void PossessedBy(AController* NewController) override; 
	virtual void OnRep_PlayerState() override; 

private:
	void InitAbilityActorInfo(); 
};
