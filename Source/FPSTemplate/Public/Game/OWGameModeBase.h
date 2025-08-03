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

	void CleanupOldPawns(); 

	FOnGameModePlayerInitialized OnGameModePlayerInitialized; 

	FOnGameplayReady OnGameplayReady; 

protected:
	UPROPERTY()
	TArray<APlayerController*> PendingPlayers; 

private:
	FOWGamePhaseTagDelegate FirstHeroSelectionEndedDelegate; 
	FOWGamePhaseTagDelegate SwitchInningEndedDelegate; 
	FOWGamePhaseTagDelegate SecondHeroSelectionEndedDelegate; 

	UFUNCTION()
	void HandleFirstHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration); 

	UFUNCTION()
	void HandleSwitchInningPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSecondHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	void EnableHeroSpawning(); 

	void RestartHeroWithClass(APlayerController* PC, TSubclassOf<APawn> PawnClass); 

	bool bCanSpawnHero = false; 
};
