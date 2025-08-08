// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacter.h"
#include "TracerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API ATracerCharacter : public AOWCharacter
{
	GENERATED_BODY()
	
public:
	ATracerCharacter(); 

	virtual FVector GetProjectileStartLocation_Implementation() const override; 

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> PulseBomb; 
};
