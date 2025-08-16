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
class APlayerController; 
class UHeroSelectionOverlay; 

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	float RespawnTime = 10.f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHeroSelectionOverlay> HeroSelectionOverlayWidgetClass; 

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	UFUNCTION()
	void ChangeSpectator();

	void GetAllLiveTeamMembers(TArray<APawn*>& OutControllers) const;

	UPROPERTY()
	int32 CurrentWatchIndex = -1; 
};
