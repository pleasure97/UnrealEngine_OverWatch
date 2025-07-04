// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/OWMessageTypes.h"
#include "DamageMarker.generated.h"

class UImage; 
class UWidgetAnimation; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UDamageMarker : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_DamageMarker; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_KillIcon; 

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DamageAnimation; 

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> KillAnimation;

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	FGameplayMessageListenerHandle DamageListenerHandle; 
	FGameplayMessageListenerHandle KillListenerHandle; 

	UFUNCTION()
	void OnEnemyDamaged(FGameplayTag Channel, const FHeroDamagedInfo& Payload); 

	UFUNCTION()
	void OnEnemyKilled(FGameplayTag Channel, const FOWVerbMessage& Payload);
};
