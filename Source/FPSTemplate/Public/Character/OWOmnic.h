// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacterBase.h"
#include "OWOmnic.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWOmnic : public AOWCharacterBase
{
	GENERATED_BODY()
	
public:
	AOWOmnic(); 
	
protected:
	virtual void BeginPlay() override; 
	
};
