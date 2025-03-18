// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/CombatInterface.h"
#include "GameplayTagContainer.h"
#include "OWCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage; 
class UGameplayAbility; 
class UDebuffNiagaraComponent; 
class UGameplayEffect; 
class UHealthBarPoolWidgetComponent;

UCLASS(ABSTRACT)
class FPSTEMPLATE_API AOWCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AOWCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; 
	UAttributeSet* GetAttributeSet() const { return AttributeSet;  }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const; 

	/** Combat Interface **/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override; 
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override; 
	virtual FOnDeath& GetOnDeathDelegate() override; 
	virtual void Die(const FVector& DeathImpulse) override; 
	virtual bool IsDead_Implementation() const override; 
	virtual bool IsBeingShocked_Implementation() const override;
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override; 
	virtual bool IsBeingHealed_Implementation() const override;
	virtual void SetIsBeingHealed_Implementation(bool bInHeal) override; 
	virtual void SetInHealLoop_Implementation(bool bInLoop) override; 
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override; 

	FOnASCRegistered OnASCRegistered; 
	FOnDeath OnDeath; 
	/** Combat Interface End **/

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse); 

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsStunned = false;

protected:
	virtual void BeginPlay() override;
	
	void AddHeroAbilities(); 

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon; 

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; 

	/* Attributes */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes; 

	virtual void InitializeDefaultAttributes() const; 

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const; 

	/* State - Death */
	UPROPERTY()
	bool bDead = false; 

	/* State - Stun */
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount); 

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 550.f; 

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;

	/* State - Heal */
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingHealed = false;

	/* Widget Component */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHealthBarPoolWidgetComponent> HealthBarPoolWidgetComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
