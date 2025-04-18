// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSpawningManagerComponent.generated.h"

class APlayerStart; 
class AOWGameModeBase; 
class AOWPlayerStart; 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTEMPLATE_API UPlayerSpawningManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AOWGameModeBase; 

public:	
	UPlayerSpawningManagerComponent();

	virtual void InitializeComponent() override; 
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<AOWPlayerStart*>& PlayerStarts); 
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) { return; }

	AOWPlayerStart* GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<AOWPlayerStart*>& FoundStartPoints) const; 

private:
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AOWPlayerStart>> CachedPlayerStarts; 

	AActor* ChoosePlayerStart(AController* Player); 
	bool ControllerCanRestart(AController* Player); 
	void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation); 

	void OnLevelAdded(ULevel* InLevel, UWorld* InWorld); 
	void HandleOnActorSpawned(AActor* SpawnedActor); 

#if WITH_EDITOR
	AOWPlayerStart* FindPlayFromHereStart(AController* Player); 
#endif
};
