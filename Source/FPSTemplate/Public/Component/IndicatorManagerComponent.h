// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IndicatorManagerComponent.generated.h"

class UIndicatorController; 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTEMPLATE_API UIndicatorManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIndicatorManagerComponent();

	static UIndicatorManagerComponent* GetComponent(AController* Controller); 

	UFUNCTION(BlueprintCallable)
	void AddIndicator(UIndicatorController* Indicator); 

	UFUNCTION(BlueprintCallable)
	void RemoveIndicator(UIndicatorController* Indicator);

	DECLARE_EVENT_OneParam(UIndicatorManagerComponent, FIndicatorEvent, UIndicatorController* Indicator)
	FIndicatorEvent OnIndicatorAdded; 
	FIndicatorEvent OnIndicatorRemoved; 

	const TArray<UIndicatorController*>& GetIndicators() const { return Indicators; }

protected:
	virtual void BeginPlay() override; 

private:
	UPROPERTY()
	TArray<UIndicatorController*> Indicators; 
};
