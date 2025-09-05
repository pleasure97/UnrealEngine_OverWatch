// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "HealingSunStone.generated.h"

class UBoxComponent; 
class UWidgetComponent; 
class UNiagaraComponent; 
class UGameplayEffect; 
class UGameplayAbility; 
class UNiagaraSystem; 
class USoundBase; 
class UOWAbilitySystemComponent;
class UOWAttributeSet; 
struct FGameplayEffectSpec; 

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
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SunStone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> SunRay; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> HealingRay;

	/* Vital Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> VitalAttributes; 

	/* Gameplay Ability */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> HealingSunStoneAbilityClass; 

	/* Spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CooldownWhenNotDamaged = 6.f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CooldownWhenDamaged = 14.f;

	UFUNCTION(BlueprintCallable)
	void Throw(FVector NewVelocity); 

	/* Ability System Interface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; 

	UFUNCTION(BlueprintCallable)
	UOWAttributeSet* GetAttributeSet() const; 

protected:
	virtual void BeginPlay() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
	void InitAbilityActorInfo(); 
	void InitializeVitalAttributes(); 
	void InitializeHealingAbility(); 
	void InitializeHealthPlate(); 

	/* Heal Mechanics */
	UFUNCTION(BlueprintCallable)
	void OnAttached(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	void OnDestroyed(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float Level = 1.f;

	/* Ability System */
	UPROPERTY()
	TObjectPtr<UOWAbilitySystemComponent> AbilitySystemComponent; 

	UPROPERTY()
	TObjectPtr<UOWAttributeSet> AttributeSet; 
};
