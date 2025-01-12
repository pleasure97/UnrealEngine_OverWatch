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

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UOWAttributeSet(); 

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health; 
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, Health); 
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Vital Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxArmor, Category = "Vital Attributes")
	FGameplayAttributeData MaxArmor;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxArmor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Vital Attributes")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, Shield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxShield, Category = "Vital Attributes")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UOWAttributeSet, MaxShield);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const; 

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const; 

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor) const;

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldShield) const;

	UFUNCTION()
	void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const;
};
