// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "HeroChangeZone.generated.h"

class APlayerController; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AHeroChangeZone : public ATriggerBox
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
	TSubclassOf<UGameplayAbility> GameplayAbility_ChangeHero; 
	
protected:
	virtual void BeginPlay() override; 

private:
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor); 

	UFUNCTION()	
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor); 

	UPROPERTY()
	TMap<APlayerController*, FGameplayAbilitySpecHandle> GrantedHandles; 
};
