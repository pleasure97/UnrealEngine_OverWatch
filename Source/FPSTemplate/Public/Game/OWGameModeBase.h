// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DS_MatchGameMode.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/Data/OmnicInfo.h"
#include "OWGameModeBase.generated.h"

class AOWCharacter; 

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameModePlayerInitialized, AGameModeBase*,  AController*);

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWGameModeBase : public ADS_MatchGameMode
{
	GENERATED_BODY()
	
public:
	/* Data Asset */
	UPROPERTY(EditDefaultsOnly, Category = "Hero Information")
	TObjectPtr<UHeroInfo> HeroInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Hero Information")
	TMap<EHeroName, TSubclassOf<AOWCharacter>> HeroMap; 

	UPROPERTY(EditDefaultsOnly, Category = "Hero Information")
	TObjectPtr<UOmnicInfo> OmnicInfo;

	virtual void BeginPlay() override; 

	/** AGameModeBase Interface **/
	//virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void GenericPlayerInitialization(AController* NewPlayer) override; 
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override; 
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override; 
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override; 
	virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override; 
	virtual bool UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage) override; 
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override; 
	/** AGameModeBase Interface End **/

	UFUNCTION(BlueprintCallable)
	void RequestPlayerRestartNextFrame(AController* Controller); 

	virtual bool ControllerCanRestart(AController* Controller); 

	virtual void ChangeHero(APlayerController* PlayerController, EHeroName NewHeroName);

	void PlayerDied(ACharacter* DeadCharacter);

	FOnGameModePlayerInitialized OnGameModePlayerInitialized; 
};
