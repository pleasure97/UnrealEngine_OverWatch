// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "OWAbilityTypes.h"
#include "HealingSunStone.generated.h"

class UBoxComponent; 
class UWidgetComponent; 
class UNiagaraComponent; 
class UGameplayEffect; 
class UNiagaraSystem; 
class USoundBase; 

UCLASS()
class FPSTEMPLATE_API AHealingSunStone : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AHealingSunStone();
	
	/* Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Pedestal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SunStone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> SunRay; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> HealLaser; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> HealSound; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> HealCross;

	/* Vital Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> VitalAttributes; 

	/* Attribute Changed Delegate */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnShieldChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxShieldChanged;

	/* Spawn */
	void Throw(FVector NewVelocity); 

	/* Damage Effect */
	FDamageEffectParams DamageEffectParams; 

	/* Ability System Interface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; 

	/* Combat Interface */
	// virtual void Die(const FVector& DeathImpulse) override; 
protected:
	virtual void BeginPlay() override; 
	void InitAbilityActorInfo(); 
	void InitializeVitalAttributes(); 

	/* Heal Mechanics */
	UFUNCTION(BlueprintCallable)
	void OnAttached(
		UPrimitiveComponent* HitComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, 
		const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void ActivateHealing(); 

	UFUNCTION(BlueprintCallable)
	void FindAlliance();

	void FindAllyWithLowestHealth(UAbilitySystemComponent* ASC);

	UFUNCTION()
	void ResetCanHeal(); 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float Level = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealRadius = 1500.f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealAmount = -40.f; 

	UPROPERTY()
	float LowestHP = -1.f; 

	UPROPERTY()
	float LowestHPPercent = 1.f; 

	UPROPERTY()
	TObjectPtr<AActor> AllyToHeal; 

	bool bCanHeal = true; 

	/* Ability System */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent; 

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; 

	/* Combat Interface */
	// FOnASCRegistered OnASCRegistered;
	// FOnDamageSignature OnDamage; 
};
