// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystem/OWAttributeSet.h"

struct OWDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield); 

	OWDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOWAttributeSet, Armor, Target, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOWAttributeSet, Shield, Target, false); 
	}
};

static const OWDamageStatics& DamageStatics()
{
	static OWDamageStatics DStatics;
	return DStatics; 
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef); 
	RelevantAttributesToCapture.Add(DamageStatics().ShieldDef); 
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
}
