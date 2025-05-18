// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthPlateSourceComponent.generated.h"

struct FGameplayTag; 
struct FHealthPlateManagerInfo;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTEMPLATE_API UHealthPlateSourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthPlateSourceComponent();

	UFUNCTION()
	void OnHealthPlateDiscovered(FGameplayTag Channel, const FHealthPlateManagerInfo& Payload);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
};
