// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthPlateManagerComponent.generated.h"

class APawn;
class UAsyncAction_ListenForGameplayMessage; 
struct FGameplayTag; 
struct FHealthPlateInfo; 
class UUserWidget; 
class UIndicatorController;

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class FPSTEMPLATE_API UHealthPlateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthPlateManagerComponent();

	UFUNCTION()
	void OnHealthPlateAdded(FGameplayTag Channel, const FHealthPlateInfo& Payload);

	UFUNCTION()
	void OnHealthPlateRemoved(FGameplayTag Channel, const FHealthPlateInfo& Payload);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> IndicatorControllerClass; 

	void RegisterHealthPlateSource(APawn* IndicatedPawn); 
	void UnregisterHealthPlateSource(APawn* IndicatedPawn); 

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 

private:
	TMap<APawn*, UIndicatorController*> IndicatorMap; 
};
