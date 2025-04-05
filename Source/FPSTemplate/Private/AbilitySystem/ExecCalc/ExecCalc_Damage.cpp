// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "OWGameplayTags.h"
#include "Interfaces/CombatInterface.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Team/OWTeamSubsystem.h"

struct OWDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(LaserResistance); 

	OWDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOWAttributeSet, Armor, Target, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOWAttributeSet, Shield, Target, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOWAttributeSet, CriticalHitDamage, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOWAttributeSet, LaserResistance, Target, false); 
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
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef); 
	RelevantAttributesToCapture.Add(DamageStatics().LaserResistanceDef); 
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefinitions; 

	const FOWGameplayTags& Tags = FOWGameplayTags::Get();

	TagsToCaptureDefinitions.Add(Tags.Attributes_Defense_Armor, DamageStatics().ArmorDef); 
	TagsToCaptureDefinitions.Add(Tags.Attributes_Defense_Shield, DamageStatics().ShieldDef); 
	TagsToCaptureDefinitions.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);

	TagsToCaptureDefinitions.Add(Tags.Attributes_Resistance_Laser, DamageStatics().LaserResistanceDef); 

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent(); 
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent(); 

	AActor* SourceAvatarActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr; 
	AActor* TargetAvatarActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr; 

	int32 SourceCharacterLevel = 1; 
	if (SourceAvatarActor->Implements<UCombatInterface>())
	{
		SourceCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(SourceAvatarActor); 
	}

	int32 TargetCharacterLevel = 1; 
	if (TargetAvatarActor->Implements<UCombatInterface>())
	{
		TargetCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(TargetAvatarActor); 
	}

	const FGameplayEffectSpec& GameplayEffectSpec = ExecutionParams.GetOwningSpec(); 
	FGameplayEffectContextHandle GameplayEffectContextHandle = GameplayEffectSpec.GetContext(); 

	const FGameplayTagContainer* SourceTags = GameplayEffectSpec.CapturedSourceTags.GetAggregatedTags(); 
	const FGameplayTagContainer* TargetTags = GameplayEffectSpec.CapturedTargetTags.GetAggregatedTags(); 
	FAggregatorEvaluateParameters EvaluationParameters; 
	EvaluationParameters.SourceTags = SourceTags; 
	EvaluationParameters.TargetTags = TargetTags; 


	// Debuff
	DetermineDebuff(ExecutionParams, GameplayEffectSpec, EvaluationParameters, TagsToCaptureDefinitions); 

	// Get Damage Set By Caller Magnitude 
	float Damage = 0.f; 
	for (const FGameplayTag& DamageTypeTag : FOWGameplayTags::Get().DamageTypes)
	{
		float DamageTypeValue = GameplayEffectSpec.GetSetByCallerMagnitude(DamageTypeTag, false); 

		if (UOWAbilitySystemLibrary::bIsRadialDamage(GameplayEffectContextHandle))
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatarActor))
			{
				CombatInterface->GetOnDamageSignature().AddLambda(
					[&](float DamageAmount)
					{
						DamageTypeValue = DamageAmount;
					}
				); 
			}
			UGameplayStatics::ApplyRadialDamage(
				TargetAvatarActor,
				DamageTypeValue,
				UOWAbilitySystemLibrary::GetRadialDamageOrigin(GameplayEffectContextHandle),
				UOWAbilitySystemLibrary::GetRadialDamageRadius(GameplayEffectContextHandle),
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatarActor); 
		}

		Damage += DamageTypeValue; 
	}

	float SourceCriticalHitDamage = 0.f; 
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage); 
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f); 

	UOWAbilitySystemLibrary::SetIsCriticalHit(GameplayEffectContextHandle, SourceCriticalHitDamage > 0.f); 

	Damage = (SourceCriticalHitDamage > 0.f) ? SourceCriticalHitDamage : Damage; 

	// Cause Damage 
	float DamageAllowedMultiplier = 0.f;
	if (TargetAvatarActor)
	{
		UOWTeamSubsystem* TeamSubsystem = TargetAvatarActor->GetWorld()->GetSubsystem<UOWTeamSubsystem>();
		if (ensure(TeamSubsystem))
		{
			const bool bSameTeam = !TeamSubsystem->CanCauseDamage(SourceAvatarActor, TargetAvatarActor) ? 1. : 0.;
			if ((Damage < 0.f && bSameTeam) || (Damage > 0.f && !bSameTeam))
			{
				DamageAllowedMultiplier = 1.f; 
			}
		}
	}

	const float DamageDone = Damage * DamageAllowedMultiplier; 
	const FGameplayModifierEvaluatedData EvaluatedDamage(UOWAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, DamageDone); 

	const float DamageAbsolute = FMath::Abs(DamageDone); 
	const FGameplayModifierEvaluatedData EvaluatedUltimateGauge(UOWAttributeSet::GetUltimateGaugeAttribute(), EGameplayModOp::Additive, DamageAbsolute); 
}

void UExecCalc_Damage::DetermineDebuff(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	const FGameplayEffectSpec& Spec,
	FAggregatorEvaluateParameters EvaluationParameters, 
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
	for (const FGameplayTag& DamageType : GameplayTags.DamageTypes)
	{
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, 0.f);
		if (TypeDamage > 0.f)
		{
			const float DebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f); 
			const bool bDebuff = FMath::RandRange(1, 100) < DebuffChance; 
			if (bDebuff)
			{
				FGameplayEffectContextHandle GameplayEffectContextHandle = Spec.GetContext(); 

				UOWAbilitySystemLibrary::SetIsSuccessfulDebuff(GameplayEffectContextHandle, true); 
				UOWAbilitySystemLibrary::SetDamageType(GameplayEffectContextHandle, DamageType); 

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, 0.f); 
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f); 
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f); 

				UOWAbilitySystemLibrary::SetDebuffDamage(GameplayEffectContextHandle, DebuffDamage); 
				UOWAbilitySystemLibrary::SetDebuffDuration(GameplayEffectContextHandle, DebuffDuration); 
				UOWAbilitySystemLibrary::SetDebuffFrequency(GameplayEffectContextHandle, DebuffFrequency); 
			}

		}
	}
}
