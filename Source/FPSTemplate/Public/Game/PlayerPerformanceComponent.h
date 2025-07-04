// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/OWMessageTypes.h"
#include "PlayerPerformanceComponent.generated.h"

class APlayerState; 

USTRUCT()
struct FPlayerPerformanceTracking
{
	GENERATED_BODY()

	TMap<APlayerState*, TArray<FHeroDamagedInfo>> AccumulatedDamageByPlayer; 

	TMap<APlayerState*, TArray<FHeroDebuffedInfo>> AccumulatedDebuffByPlayer;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTEMPLATE_API UPlayerPerformanceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/* Actor Component */
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	/* Actor Component End */

	void StartPerformanceListening(); 

private:
	void AddListenerHandle(FGameplayMessageListenerHandle&& Handle);

	/* Prune Performance History */
	double GetServerTime() const; 

	void PrunePerformanceHistory(); 

	FTimerHandle PruneTimerHandle; 
	/* Prune Performance History End */

	/* Gameplay Message Callback */
	UFUNCTION()
	void OnPlayerDamagedMessage(FGameplayTag Channel, const FHeroDamagedInfo& Payload); 

	UFUNCTION()
	void OnPlayerDebuffedMessage(FGameplayTag Channel, const FHeroDebuffedInfo& Payload);

	UFUNCTION()
	void OnPlayerKilledMessage(FGameplayTag Channel, const FOWVerbMessage& Payload); 
	/* Gameplay Message Callback End */

	float PruneTime = 5.f; 

	UPROPERTY()
	TMap<TObjectPtr<APlayerState>, FPlayerPerformanceTracking> PerformanceHistory; 

	TArray<FGameplayMessageListenerHandle> ListenerHandles; 
};
