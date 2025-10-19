// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "Message/OWMessageTypes.h"
#include "OpenWorldGameState.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOpenWorldGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AOpenWorldGameState(); 

	/* Data Asset - Info */
	UPROPERTY(EditDefaultsOnly, Category = "Hero Information")
	TObjectPtr<UHeroInfo> HeroInfo;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastReliableMessageToClients(const FOWVerbMessage Message);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OpenWorldAbilitySytsemComponent; 
};
