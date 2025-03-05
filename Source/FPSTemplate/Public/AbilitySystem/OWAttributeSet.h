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

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle; 

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr; 

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr; 

	UPROPERTY()
	AController* SourceController = nullptr; 

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr; 

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr; 

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr; 

	UPROPERTY()
	AController* TargetController = nullptr; 

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr; 
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
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override; 
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override; 

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes; 

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

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const; 
};
