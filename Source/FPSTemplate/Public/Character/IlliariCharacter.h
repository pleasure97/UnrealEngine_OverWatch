// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacter.h"
#include "IlliariCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AIlliariCharacter : public AOWCharacter
{
	GENERATED_BODY()
	
public:
	AIlliariCharacter(); 

	virtual FVector GetProjectileStartLocation_Implementation() const override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> HealingPylon;
};
