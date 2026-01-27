// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DS_MatchGameMode.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "OWGameModeBase.generated.h"

class AOWCharacter; 

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameModePlayerInitialized, AGameModeBase*,  AController*)
DECLARE_MULTICAST_DELEGATE(FOnGameplayReady)

USTRUCT(BlueprintType)
struct FHeroPoolUnit
{
	GENERATED_BODY()

	UPROPERTY()
	APawn* HeroPawn = nullptr;

	UPROPERTY()
	FTransform HeroTransform;
};

USTRUCT(BlueprintType)
struct FHeroList
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FHeroPoolUnit> Heroes;
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWGameModeBase : public ADS_MatchGameMode
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override; 

	/** AGameModeBase Interface **/
	virtual void GenericPlayerInitialization(AController* NewPlayer) override; 
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override; 
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override; 
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override; 
	virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override; 
	virtual void StartPlay() override; 
	virtual void PostLogin(APlayerController* NewPlayer) override; 
	/** AGameModeBase Interface End **/

	UFUNCTION(BlueprintCallable)
	void RequestPlayerRestartNextFrame(AController* Controller); 

	virtual bool ControllerCanRestart(AController* Controller); 

	virtual void ChangeHero(APlayerController* PlayerController, EHeroName NewHeroName);

	UFUNCTION(BlueprintCallable)
	void RestartHero(APlayerController* PendingPlayerController);

	UFUNCTION(BlueprintCallable)
	AOWPlayerStart* RestartHeroAtPlayerStart(APlayerController* PlayerController);

	FOnGameModePlayerInitialized OnGameModePlayerInitialized; 

	FOnGameplayReady OnGameplayReady; 

protected:
	UPROPERTY()
	TArray<APlayerController*> PendingPlayers; 

	UPROPERTY()
	TMap<EHeroName, FHeroList> HeroPool;

	void InitializeHeroPool();

	void DeactivateHero(AOWCharacter* Hero);

private:
	FOWGamePhaseTagDelegate FirstHeroSelectionStartedDelegate; 
	FOWGamePhaseTagDelegate SwitchInningStartedDelegate;
	FOWGamePhaseTagDelegate SwitchInningEndedDelegate; 
	FOWGamePhaseTagDelegate SecondHeroSelectionStartedDelegate; 
	FOWGamePhaseTagDelegate SecondHeroSelectionEndedDelegate;

	UFUNCTION()
	void HandleWhenFirstHeroSelectionEnds(const FGameplayTag& PhaseTag, const float PhaseDuration); 

	UFUNCTION()
	void HandleWhenSwitchInningStarts(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleWhenSwitchInningEnds(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleWhenSecondHeroSelectionStarts(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleWhenSecondHeroSelectionEnds(const FGameplayTag& PhaseTag, const float PhaseDuration);

	void EnableHeroSpawning();

	void ActivateHeroFromPool(APlayerController* PC, EHeroName HeroName);

	void PushHeroesToHeroPool();

	void PushHeroToHeroPool(AOWCharacter* HeroToPush);

	bool bCanSpawnHero = false; 
};
