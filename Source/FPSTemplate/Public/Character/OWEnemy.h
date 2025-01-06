// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacterBase.h"
#include "OWEnemy.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWEnemy : public AOWCharacterBase
{
	GENERATED_BODY()
	
public:
	AOWEnemy(); 
	
protected:
	virtual void BeginPlay() override; 
	
};
