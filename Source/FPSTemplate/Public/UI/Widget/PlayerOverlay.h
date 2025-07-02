// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/OWMessageTypes.h"
#include "PlayerOverlay.generated.h"

class UHealthBarPool; 
class UPlayerHealthStatus; 
class UPlayerSkills; 
class UPlayerWeaponStatus; 
class UUltimateGauge; 
class UHitIndicatorPool; 
class UCombatLogPool; 
class UKillLogPool;
class UAssaultOverlay; 
class UDamageMarker; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPlayerOverlay : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	/* Subwidgets */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHealthBarPool> WBP_PlayerHealthBarPool; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPlayerHealthStatus> WBP_PlayerHealthStatus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPlayerSkills> WBP_PlayerSkills;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPlayerWeaponStatus> WBP_PlayerWeaponStatus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUltimateGauge> WBP_UltimateGauge;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHitIndicatorPool> WBP_HitIndicatorPool; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCombatLogPool> WBP_CombatLogPool; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UKillLogPool> WBP_KillLogPool;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UAssaultOverlay> WBP_AssaultOverlay; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDamageMarker> WBP_DamageMarker; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_GameplayMessage; 

	virtual void SetChildWidgetControllers() override; 

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	UFUNCTION()
	void OnPlayerDebuffedMessage(FGameplayTag Channel, const FHeroDebuffedInfo& Payload); 

	UFUNCTION()
	void OnMatchBeginCountdownMessage(FGameplayTag Channel, const FOWVerbMessage& Payload);

	void AddListenerHandle(FGameplayMessageListenerHandle&& Handle);

	TArray<FGameplayMessageListenerHandle> ListenerHandles;
};
