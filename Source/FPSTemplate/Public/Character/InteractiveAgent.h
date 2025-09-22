// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/InteractInterface.h"
#include "InteractiveAgent.generated.h"

UCLASS()
class FPSTEMPLATE_API AInteractiveAgent : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	AInteractiveAgent();

	virtual void AddInteractionOptions(const FInteractionQuery& InteractionQuery, FInteractionOptionCollector& InteractionOptionCollector) override;

protected:
	UPROPERTY(EditAnywhere)
	FInteractionOption InteractionOption; 
};
