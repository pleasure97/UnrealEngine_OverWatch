// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "OWGlobalAbilitySystem.generated.h"

class UOWAbilitySystemComponent;
class UGameplayAbility; 
class UGameplayEffect;


USTRUCT()
struct FOWGlobalAbilityList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UOWAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles; 

	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UOWAbilitySystemComponent* ASC); 
	void RemoveFromASC(UOWAbilitySystemComponent* ASC); 
	void RemoveFromAll(); 
};

USTRUCT()
struct FOWGlobalEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UOWAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UOWAbilitySystemComponent* ASC); 
	void RemoveFromASC(UOWAbilitySystemComponent* ASC); 
	void RemoveFromAll(); 
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UOWGlobalAbilitySystem(); 

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Global Ability System")
	void ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect); 

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Global Ability System")
	void RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect); 

	// Register an Ability System Component and Apply Any Active Global Effects/Abilities. 
	void RegisterASC(UOWAbilitySystemComponent* ASC); 

	// Remove an Ability System Component, along with Any Active Global Effects/Abilities. 
	void UnregisterASC(UOWAbilitySystemComponent* ASC); 
private:
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FOWGlobalAbilityList> AppliedAbilities; 

	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FOWGlobalEffectList> AppliedEffects; 

	UPROPERTY()
	TArray<TObjectPtr<UOWAbilitySystemComponent>> RegisteredASCs; 
};
