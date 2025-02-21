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

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes; 

	/* 
	 * Defensive Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Defensive Attributes", Meta=(GameplayTag="Attributes.Defense.Health"))
	FGameplayAttributeData Health; 
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, Health); 
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Defensive Attributes", Meta=(GameplayTag="Attributes.Defense.MaxHealth"))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Defensive Attributes", Meta=(GameplayTag="Attributes.Defense.Armor"))
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxArmor, Category="Defensive Attributes", Meta=(GameplayTag="Attributes.Defense.MaxArmor"))
	FGameplayAttributeData MaxArmor;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxArmor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_TempArmor, Category="Defensive Attributes", Meta=(GameplayTag="Attributes.Defense.TempArmor"))
	FGameplayAttributeData TempArmor;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, TempArmor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Shield, Category="Defensive Attributes", Meta=(GameplayTag="Attributes.Defense.Shield"))
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, Shield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxShield, Category="Defensive Attributes", Meta=(GameplayTag="Attributes.Defense.MaxShield"))
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxShield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_TempShield, Category="Defensive Attributes", Meta=(GameplayTag="Attributes.Defense.TempShield"))
	FGameplayAttributeData TempShield;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, TempShield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_OverHealth, Category="Defensive Attributes", Meta=(GameplayTag="Attributes.Defense.OverHealth"))
	FGameplayAttributeData OverHealth;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, OverHealth);

	/*
	 * Skill Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SkillGauge, Category="Skill Attributes", Meta=(GameplayTag="Attributes.Skill.SkillGauge"))
	FGameplayAttributeData SkillGauge;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, SkillGauge);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_UltimateGauge, Category="Skill Attributes", Meta=(GameplayTag="Attributes.Skill.UltimateGauge"))
	FGameplayAttributeData UltimateGauge;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, UltimateGauge);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxUltimateGauge, Category="Skill Attributes", Meta=(GameplayTag="Attributes.Skill.MaxUltimateGauge"))
	FGameplayAttributeData MaxUltimateGauge;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxUltimateGauge);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_NumCurrentBullets, Category="Skill Attributes", Meta=(GameplayTag = "Attributes.Skill.NumCurrentBullets"))
	FGameplayAttributeData NumCurrentBullets;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, NumCurrentBullets);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_NumMaxBullets, Category="Skill Attributes", Meta=(GameplayTag="Attributes.Skill.NumMaxBullets"))
	FGameplayAttributeData NumMaxBullets;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, NumMaxBullets);


	/*
	 * Defensive Attributes
	 */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const; 

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const; 

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor) const;

	UFUNCTION()
	void OnRep_TempArmor(const FGameplayAttributeData& OldTempArmor) const;

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldShield) const;

	UFUNCTION()
	void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const;

	UFUNCTION()
	void OnRep_TempShield(const FGameplayAttributeData& OldTempShield) const;

	UFUNCTION()
	void OnRep_OverHealth(const FGameplayAttributeData& OldOverHealth) const;

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
};
