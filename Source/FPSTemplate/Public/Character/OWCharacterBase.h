// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/CombatInterface.h"
#include "OWCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage; 
class UGameplayAbility; 

UCLASS(ABSTRACT)
class FPSTEMPLATE_API AOWCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AOWCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; 
	UAttributeSet* GetAttributeSet() const { return AttributeSet;  }

	/** Combat Interface **/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override; 
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override; 
	virtual FOnDeath& GetOnDeathDelegate() override; 
	virtual void Die(const FVector& DeathImpulse) override; 
	virtual bool IsDead_Implementation() const override; 

	FOnASCRegistered OnASCRegistered; 
	FOnDeath OnDeath; 
	/** Combat Interface End **/

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse); 

protected:
	virtual void BeginPlay() override;
	
	void AddHeroAbilities(); 

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon; 

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; 

	UPROPERTY()
	bool bDead = false; 

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities; 
};
