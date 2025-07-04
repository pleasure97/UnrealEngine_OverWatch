// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/OWHUD.h"
#include "Player/OWPlayerState.h"
#include "UI/WidgetController/OWWidgetController.h"
#include "AbilitySystemComponent.h"
#include "Game/OWGameModeBase.h"
#include "OWGameplayTags.h"
#include "OWAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/OverlapResult.h"
#include "Interface/CombatInterface.h"
#include "Game/OWGameState.h"

/* Widget Controller */
bool UOWAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AOWHUD*& OutOWHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutOWHUD = Cast<AOWHUD>(PC->GetHUD()); 
		if (OutOWHUD)
		{
			AOWPlayerState* PS = PC->GetPlayerState<AOWPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent(); 
			UAttributeSet* AS = PS->GetAttributeSet(); 

			OutWCParams.PlayerController = PC; 
			OutWCParams.PlayerState = PS; 
			OutWCParams.AbilitySystemComponent = ASC; 
			OutWCParams.AttributeSet = AS; 

			return true; 
		}
	}
	return false;
}

UOverlayWidgetController* UOWAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams; 
	AOWHUD* OWHUD = nullptr; 
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, OWHUD))
	{
		return OWHUD->GetOverlayWidgetController(WidgetControllerParams); 
	}

	return nullptr;
}

/* Hero Info Defaults */
void UOWAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, EHeroName HeroName, UAbilitySystemComponent* ASC, float Level)
{
	// Get Avatar Actor
	AActor* AvatarActor = ASC->GetAvatarActor(); 

	// Get Hero Info Data Asset
	UHeroInfo* HeroInfo = GetHeroInfo(WorldContextObject); 

	// Make Gaemplay Effect Context - Vital Attributes
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext(); 
	// Set Gameplay Effect Context's Source Object to Avatar Actor
	VitalAttributesContextHandle.AddSourceObject(AvatarActor); 
	// Make Gameplay Effect Spec - Vital Attributes 
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(
		HeroInfo->HeroInformation[HeroName].VitalAttributes, Level, VitalAttributesContextHandle); 
	// Apply Gameplay Effect Spec to Self
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get()); 

	// Same as above - Primary Attributes
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext(); 
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor); 
	EHeroClass HeroClass = HeroInfo->HeroInformation[HeroName].HeroClass; 
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		HeroInfo->CommonClassInformation[HeroClass].PrimaryAttributes, Level, PrimaryAttributesContextHandle); 
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get()); 

	// Same as above - Common Attributes
	for (TSubclassOf<UGameplayEffect> CommonAttribute : HeroInfo->CommonAttributes)
	{
		FGameplayEffectContextHandle CommonAttributeContextHandle = ASC->MakeEffectContext();
		CommonAttributeContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle CommonAttributeSpecHandle = ASC->MakeOutgoingSpec(
			CommonAttribute, Level, CommonAttributeContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*CommonAttributeSpecHandle.Data.Get());
	}
}

void UOWAbilitySystemLibrary::GiveDefaultAbilities(const UObject* WorldContextObject, EHeroName HeroName, UAbilitySystemComponent* ASC)
{
	UHeroInfo* HeroInfo = GetHeroInfo(WorldContextObject); 
	if (!HeroInfo) return; 

	for (TSubclassOf<UGameplayAbility> AbilityClass : HeroInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1); 
		ASC->GiveAbility(AbilitySpec); 
	}
}

UHeroInfo* UOWAbilitySystemLibrary::GetHeroInfo(const UObject* WorldContextObject)
{
	const AOWGameState* OWGameState = Cast<AOWGameState>(UGameplayStatics::GetGameState(WorldContextObject)); 
	if (OWGameState == nullptr)
	{
		return nullptr; 
	}
	return OWGameState->HeroInfo;
}

EHeroName UOWAbilitySystemLibrary::GetHeroName(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return EHeroName::None;

	const AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(UGameplayStatics::GetPlayerState(WorldContextObject, 0));
	return OWPlayerState ? OWPlayerState->GetHeroName() : EHeroName::None;
}

void UOWAbilitySystemLibrary::GetIndividualHeroInfo(const UObject* WorldContextObject, EHeroName HeroName, FOWHeroInfo& OutHeroInfo)
{
	UHeroInfo* HeroInfo = GetHeroInfo(WorldContextObject); 
	OutHeroInfo = HeroInfo->HeroInformation[HeroName]; 
}

APlayerState* UOWAbilitySystemLibrary::GetPlayerStateFromObject(UObject* Object)
{
	if (APlayerController* PC = Cast<APlayerController>(Object))
	{
		return PC->PlayerState;
	}

	if (APlayerState* TargetPlayerState = Cast<APlayerState>(Object))
	{
		return TargetPlayerState;
	}

	if (APawn* TargetPawn = Cast<APawn>(Object))
	{
		if (APlayerState* TargetPlayerState = TargetPawn->GetPlayerState())
		{
			return TargetPlayerState;
		}
	}
	return nullptr;
}

UOmnicInfo* UOWAbilitySystemLibrary::GetOmnicInfo(const UObject* WorldContextObject)
{
	const AOWGameState* OWGameState = Cast<AOWGameState>(UGameplayStatics::GetGameState(WorldContextObject)); 
	if (OWGameState == nullptr)
	{
		return nullptr; 
	}

	return OWGameState->OmnicInfo;
}

/* Effect Context Getter */
bool UOWAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->IsCriticalHit();
	}
	return false;
}

bool UOWAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UOWAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->GetDebuffDamage();
	}
	return 0.0f;
}

float UOWAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->GetDebuffDuration();
	}
	return 0.0f;
}

float UOWAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->GetDebuffFrequency();
	}
	return 0.0f;
}

FGameplayTag UOWAbilitySystemLibrary::GetDebuffTag(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return *OWEffectContext->GetDebuffTag(); 
	}
	return FGameplayTag();
}

FGameplayTag UOWAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (OWEffectContext->GetDamageType().IsValid())
		{
			return *OWEffectContext->GetDamageType(); 
		}
	}
	return FGameplayTag();
}

FVector UOWAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->GetDeathImpulse();
	}
	return FVector();
}

FVector UOWAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UOWAbilitySystemLibrary::bIsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->IsRadialDamage();
	}
	return false;
}

float UOWAbilitySystemLibrary::GetRadialDamageRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->GetRadialDamageRadius();
	}
	return 0.0f;
}

FVector UOWAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return OWEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

/* Effect Context Setter */
void UOWAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetIsCriticalHit(bInIsCriticalHit); 
	}	
}

void UOWAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UOWAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UOWAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetDebuffDuration(InDebuffDuration); 
	}
}

void UOWAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetDebuffFrequency(InDebuffFrequency); 
	}
}

void UOWAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType); 
		OWEffectContext->SetDamageType(DamageType); 
	}
}

void UOWAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UOWAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetKnockbackForce(InKnockbackForce);
	}
}

void UOWAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetIsRadialDamage(bInIsRadialDamage); 
	}
}

void UOWAbilitySystemLibrary::SetRadialDamageRadius(FGameplayEffectContextHandle& EffectContextHandle, float InRadius)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetRadialDamageRadius(InRadius); 
	}
}

void UOWAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin)
{
	if (FOWGameplayEffectContext* OWEffectContext = static_cast<FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		OWEffectContext->SetRadialDamageOrigin(InRadialDamageOrigin); 
	}
}

void UOWAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams; 
	SphereParams.AddIgnoredActors(ActorsToIgnore); 

	TArray<FOverlapResult> Overlaps; 
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, 
			SphereOrigin, 
			FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
			FCollisionShape::MakeSphere(Radius), 
			SphereParams); 
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor())); 
			}
		}
	}
}

/* Gameplay Mechanics */
FGameplayEffectContextHandle UOWAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor(); 

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext(); 
	EffectContextHandle.AddSourceObject(SourceAvatarActor); 
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse); 
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce); 

	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage); 
	SetRadialDamageRadius(EffectContextHandle, DamageEffectParams.RadialDamageRadius); 
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin); 

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
		DamageEffectParams.DamageGameplayEffectClass,
		DamageEffectParams.AbilityLevel,
		EffectContextHandle); 

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage); 
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance); 
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage); 
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration); 
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency); 

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data); 

	return EffectContextHandle; 
}

int32 UOWAbilitySystemLibrary::GetXPRewardFromClassAndLevel(const UObject* WorldContextObject, EOmnicClass OmnicClass, int32 OmnicLevel)
{
	UOmnicInfo* OmnicInfo = GetOmnicInfo(WorldContextObject); 
	if (!OmnicInfo)
	{
		return 0; 
	}
	const FOmnicClassDefaultInfo& Info = OmnicInfo->GetOmnicDefaultInfo(OmnicClass); 
	const float XPReward = Info.XPReward.GetValueAtLevel(OmnicLevel); 

	return static_cast<int32>(XPReward); 
}

void UOWAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize(); 
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude; 
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude; 
	}
}

void UOWAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
}

void UOWAbilitySystemLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InAbilitySystemComponent)
{
	DamageEffectParams.TargetAbilitySystemComponent = InAbilitySystemComponent; 
}
