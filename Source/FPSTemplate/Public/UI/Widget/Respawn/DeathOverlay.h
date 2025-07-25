// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "DeathOverlay.generated.h"

class UTextBlock;
class URespawnGauge; 
class UButton; 
class APawn; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UDeathOverlay : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_TeamSpectatorName; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ChangeSpectator;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URespawnGauge> WBP_RespawnGauge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APawn> SpectatorClass; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	float RespawnTime = 10.f; 

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	UFUNCTION()
	void ChangeSpectator();

	void WatchLiveTeamMember(); 
	void BecomeSpectator(); 
	void AttachToLiveTeamMember(APawn* TeamMember, APawn* SpectatorPawn);

	TObjectPtr<APawn> Spectator; 
	TObjectPtr<APawn> CurrentlyFollowingTeamMember; 
};
