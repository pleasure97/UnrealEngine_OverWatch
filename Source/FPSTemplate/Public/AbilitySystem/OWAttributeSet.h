// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "OWAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDamageReceived, AActor*, /* DamageEffectCauser */ AActor*, /* Owner Actor */ float /* Damage */);
DECLARE_MULTICAST_DELEGATE_FourParams(FOWAttributeEvent, AActor* /* Effect Instigator */, AActor* /* Effect Causer */, const FGameplayEffectSpec* /* Effect Spec */, float /* Effect Magnitude*/); 

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle; 

	const FGameplayEffectSpec* EffectSpec = nullptr; 

	UPROPERTY()
	float Magnitude = 0.f; 

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr; 

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr; 

	UPROPERTY()
	AController* SourceController = nullptr; 

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr; 

	UPROPERTY()
	APlayerState* SourcePlayerState = nullptr; 

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr; 

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr; 

	UPROPERTY()
	AController* TargetController = nullptr; 

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr; 

	UPROPERTY()
	APlayerState* TargetPlayerState = nullptr; 
};

// typedef is specific to the FGameplayAttribute() signature, but TStaticFuncPtr is generic to any signature chosen. 
// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr; 
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class FPSTEMPLATE_API UOWAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UOWAttributeSet(); 

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 	

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override; 
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override; 
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override; 
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override; 

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes; 

	FOnDamageReceived OnDamageReceived; 
	FOWAttributeEvent OnDeath; 

	/* 
	 * Defensive Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Defensive Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxArmor, Category = "Defensive Attributes")
	FGameplayAttributeData MaxArmor;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxArmor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxShield, Category = "Defensive Attributes")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxShield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Defensive Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Defensive Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Shield, Category="Defensive Attributes")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, Shield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TempArmor, Category = "Defensive Attributes")
	FGameplayAttributeData TempArmor;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, TempArmor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_TempShield, Category="Defensive Attributes")
	FGameplayAttributeData TempShield;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, TempShield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_OverHealth, Category="Defensive Attributes")
	FGameplayAttributeData OverHealth;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, OverHealth);

	/* 
	 * Secondary Attributes 
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage; 
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, CriticalHitDamage); 

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShieldRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ShieldRegeneration;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, ShieldRegeneration);

	/* 
	 * Resistance Attributes 
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LaserResistance, Category = "Resistance Attributes")
	FGameplayAttributeData LaserResistance; 
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, LaserResistance); 


	/*
	 * Skill Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SkillGauge, Category="Skill Attributes")
	FGameplayAttributeData SkillGauge;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, SkillGauge);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_UltimateGauge, Category="Skill Attributes")
	FGameplayAttributeData UltimateGauge;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, UltimateGauge);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxUltimateGauge, Category="Skill Attributes")
	FGameplayAttributeData MaxUltimateGauge;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxUltimateGauge);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_NumCurrentBullets, Category="Skill Attributes")
	FGameplayAttributeData NumCurrentBullets;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, NumCurrentBullets);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_NumMaxBullets, Category="Skill Attributes")
	FGameplayAttributeData NumMaxBullets;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, NumMaxBullets);

	/* 
	 * Meta Attributes 
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, IncomingDamage); 

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, IncomingXP); 

	/*
	 * Match Attributes 
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_NumKills, Category = "Match Attributes")
	FGameplayAttributeData NumKills; 
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, NumKills);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_NumDeaths, Category = "Match Attributes")
	FGameplayAttributeData NumDeaths;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, NumDeaths);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_NumAssists, Category = "Match Attributes")
	FGameplayAttributeData NumAssists;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, NumAssists);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalDamage, Category = "Match Attributes")
	FGameplayAttributeData TotalDamage;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, TotalDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalHeal, Category = "Match Attributes")
	FGameplayAttributeData TotalHeal;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, TotalHeal);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalMitigatedDamage, Category = "Match Attributes")
	FGameplayAttributeData TotalMitigatedDamage;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, TotalMitigatedDamage);

	/*------------------------------- Replicating Functions -------------------------------*/ 

	/*
	 * Defensive Attributes
	 */

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor) const;

	UFUNCTION()
	void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const; 

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldShield) const;
	
	UFUNCTION()
	void OnRep_TempArmor(const FGameplayAttributeData& OldTempArmor) const;

	UFUNCTION()
	void OnRep_TempShield(const FGameplayAttributeData& OldTempShield) const;

	UFUNCTION()
	void OnRep_OverHealth(const FGameplayAttributeData& OldOverHealth) const;

	/* 
	 * Secondary Attributes
	 */
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const; 

	UFUNCTION()
	void OnRep_ShieldRegeneration(const FGameplayAttributeData& OldShieldRegeneration) const; 

	/*
	 * Resistance Attributes
	 */
	UFUNCTION()
	void OnRep_LaserResistance(const FGameplayAttributeData& OldLaserResistance) const; 

	/*
	 * Skill Attributes
	 */
	UFUNCTION()
	void OnRep_SkillGauge(const FGameplayAttributeData& OldSkillGauge) const;

	UFUNCTION()
	void OnRep_UltimateGauge(const FGameplayAttributeData& OldUltimateGauge) const;

	UFUNCTION()
	void OnRep_MaxUltimateGauge(const FGameplayAttributeData& OldMaxUltimateGauge) const;

	UFUNCTION()
	void OnRep_NumCurrentBullets(const FGameplayAttributeData& OldNumCurrentBullets) const; 

	UFUNCTION()
	void OnRep_NumMaxBullets(const FGameplayAttributeData& OldNumMaxBullets) const;

	/*
	 * Match Attributes
	 */
	UFUNCTION()
	void OnRep_NumKills(const FGameplayAttributeData& OldNumKills) const;

	UFUNCTION()
	void OnRep_NumDeaths(const FGameplayAttributeData& OldNumDeaths) const;

	UFUNCTION()
	void OnRep_NumAssists(const FGameplayAttributeData& OldNumAssists) const;

	UFUNCTION()
	void OnRep_TotalDamage(const FGameplayAttributeData& OldTotalDamage) const;

	UFUNCTION()
	void OnRep_TotalHeal(const FGameplayAttributeData& OldTotalHeal) const;

	UFUNCTION()
	void OnRep_TotalMitigatedDamage(const FGameplayAttributeData& OldTotalMitigatedDamage) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const; 

	void HandleIncomingDamage(const FEffectProperties& EffectProperties);
	void SendHeroKilledEvent(const FEffectProperties& EffectProperties);
	void HandleIncomingXP(const FEffectProperties& EffectProperties);
	void SendXPEvent(const FEffectProperties& EffectProperties);
	void Debuff(const FEffectProperties& EffectProperties); 
};
