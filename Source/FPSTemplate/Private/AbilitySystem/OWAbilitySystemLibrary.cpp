// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/OWHUD.h"
#include "Player/OWPlayerState.h"
#include "Game/OWGameModeBase.h"
#include "OWGameplayTags.h"
#include "OWAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/OverlapResult.h"
#include "Interface/CombatInterface.h"
#include "Game/OWGameState.h"
#include "Game/OpenWorldGameState.h"
#include "Interface/InteractInterface.h"
#include "Character/OWCharacter.h"
#include "Camera/CameraComponent.h"

/* Gameplay Abilities */
UOWGameplayAbility* UOWAbilitySystemLibrary::GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass)
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* GameplayAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(InAbilityClass); 
		if (GameplayAbilitySpec)
		{
			return Cast<UOWGameplayAbility>(GameplayAbilitySpec->GetPrimaryInstance()); 
		}
	}
	return nullptr;
}

UOWGameplayAbility* UOWAbilitySystemLibrary::GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* GameplayAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle); 
		if (GameplayAbilitySpec)
		{
			return Cast<UOWGameplayAbility>(GameplayAbilitySpec->GetPrimaryInstance()); 
		}
	}
	return nullptr; 
}

bool UOWAbilitySystemLibrary::IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle)
{
	return Handle.IsValid(); 
}

/* Hero Info Defaults */
void UOWAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, EHeroName HeroName, UOWAbilitySystemComponent* ASC, float Level)
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
	FActiveGameplayEffectHandle ActiveVitalGameplayEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
	ASC->AddToDefaultAttributeHandles(ActiveVitalGameplayEffectHandle);

	// Same as above - Primary Attributes
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext(); 
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor); 
	EHeroClass HeroClass = HeroInfo->HeroInformation[HeroName].HeroClass; 
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		HeroInfo->CommonClassInformation[HeroClass].PrimaryAttributes, Level, PrimaryAttributesContextHandle); 
	FActiveGameplayEffectHandle ActivePrimaryGameplayEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());
	ASC->AddToDefaultAttributeHandles(ActivePrimaryGameplayEffectHandle);

	// Same as above - Common Attributes
	for (TSubclassOf<UGameplayEffect> CommonAttribute : HeroInfo->CommonAttributes)
	{
		FGameplayEffectContextHandle CommonAttributeContextHandle = ASC->MakeEffectContext();
		CommonAttributeContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle CommonAttributeSpecHandle = ASC->MakeOutgoingSpec(
			CommonAttribute, Level, CommonAttributeContextHandle);
		FActiveGameplayEffectHandle ActiveCommonGameplayEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*CommonAttributeSpecHandle.Data.Get());
		ASC->AddToDefaultAttributeHandles(ActiveCommonGameplayEffectHandle);
	}
}

void UOWAbilitySystemLibrary::ResetAttributes(const UObject* WorldContextObject, EHeroName HeroName, UOWAbilitySystemComponent* ASC, float Level)
{
	// Get Avatar Actor
	AActor* AvatarActor = ASC->GetAvatarActor();

	// Get Hero Info Data Asset
	UHeroInfo* HeroInfo = GetHeroInfo(WorldContextObject);

	// Make Gaemplay Effect Context - Reset Attributes
	FGameplayEffectContextHandle ResetAttributesContextHandle = ASC->MakeEffectContext();
	// Set Gameplay Effect Context's Source Object to Avatar Actor
	ResetAttributesContextHandle.AddSourceObject(AvatarActor);
	// Make Gameplay Effect Spec - Vital Attributes 
	const FGameplayEffectSpecHandle ResetAttributesSpecHandle = ASC->MakeOutgoingSpec(
		HeroInfo->HeroInformation[HeroName].ResetAttributes, Level, ResetAttributesContextHandle);
	// Apply Gameplay Effect Spec to Self
	ASC->ApplyGameplayEffectSpecToSelf(*ResetAttributesSpecHandle.Data.Get());

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
	if (const AOWGameState* OWGameState = Cast<AOWGameState>(UGameplayStatics::GetGameState(WorldContextObject)))
	{
		return OWGameState->HeroInfo;
	}
	else if (const AOpenWorldGameState* OpenWorldGameState = Cast<AOpenWorldGameState>(UGameplayStatics::GetGameState(WorldContextObject)))
	{
		return OpenWorldGameState->HeroInfo; 
	}
	else
	{
		return nullptr; 
	}
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
		return OWEffectContext->GetDebuffTag(); 
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag UOWAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOWGameplayEffectContext* OWEffectContext = static_cast<const FOWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (OWEffectContext->GetDamageType().IsValid())
		{
			return OWEffectContext->GetDamageType(); 
		}
	}
	return FGameplayTag::EmptyTag;
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
		OWEffectContext->SetDamageType(InDamageType);
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

void UOWAbilitySystemLibrary::SendGameplayEventToActors(AActor* Instigator, const FGameplayTag& InGameplayTag, const TArray<AActor*>& ActorsHit)
{
	// Iterate Hit Actors 
	for (AActor* HitActor : ActorsHit)
	{
		// Send Gameplay Event Containing Instigator Information
		FGameplayEventData Payload;
		Payload.Instigator = Instigator;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, InGameplayTag, Payload);
	}
}

TArray<AActor*> UOWAbilitySystemLibrary::CheckHitBoxOverlap(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset, float HitBoxElevationOffset, bool bDrawDebugs)
{
	if (!IsValid(AvatarActor))
	{
		return TArray<AActor*>();
	}

	// Ensure that Overlap Ignores Avatar Actor
	FCollisionQueryParams CollisionQueryParams; 
	CollisionQueryParams.AddIgnoredActor(AvatarActor); 

	// Set Collision Response Params (Pawn - Block / All Others - Ignore)
	FCollisionResponseParams CollisionResponseParams; 
	CollisionResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore); 
	CollisionResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block); 
	
	// Declare Overlap Results to Save Overlaps
	TArray<FOverlapResult> OverlapResults; 
	FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius); 

	// Set Hit Box Location 
	AOWCharacter* InstigatorCharacter = Cast<AOWCharacter>(AvatarActor);
	// Direction in which Custom Character Faces is Different from Actual Forward Vector
	FVector Forward = IsValid(InstigatorCharacter) ? InstigatorCharacter->GetFirstPersonCamera()->GetForwardVector() * HitBoxForwardOffset
													: AvatarActor->GetActorForwardVector() + HitBoxForwardOffset;
	const FVector HitBoxLocation = AvatarActor->GetActorLocation() + Forward + HitBoxElevationOffset; 

	// Get World from GEngine using Avatar Actor
	UWorld* World = GEngine->GetWorldFromContextObject(AvatarActor, EGetWorldErrorMode::LogAndReturnNull); 
	if (!IsValid(World))
	{
		return TArray<AActor*>(); 
	}

	// Trace based on Collision Query Params, and Collision Response Params, and Save to OverlapResults
	World->OverlapMultiByChannel(
		OverlapResults, HitBoxLocation, FQuat::Identity, ECC_Pawn, Sphere, CollisionQueryParams, CollisionResponseParams); 

	TArray<AActor*> ActorsHit; 
	// Iterate Overlap Results 
	for (const FOverlapResult& Result : OverlapResults)
	{
		// Cast to Custom Character and Check if the Character is Valid and Not Dead 
		AOWCharacterBase* OWCharacterBase = Cast<AOWCharacterBase>(Result.GetActor()); 
		if (!IsValid(OWCharacterBase))
		{
			continue;
		}
		if (ICombatInterface::Execute_IsDead(OWCharacterBase))
		{
			continue;
		}
		ActorsHit.AddUnique(Result.GetActor()); 
	}

	// Draw HitBox Overlap in Debug Mode 
	if (bDrawDebugs)
	{
		DrawHitBoxOverlap(AvatarActor, OverlapResults, HitBoxLocation, HitBoxRadius); 
	}

	return ActorsHit;
}

void UOWAbilitySystemLibrary::DrawHitBoxOverlap(const UObject* WorldContextObject, const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, float HitBoxRadius)
{
	// Get World and Check if World is Valid 
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull); 
	if (!IsValid(World))
	{
		return;
	}

	// Draw Hit Box Location Debug Sphere
	DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f); 

	// Iterate Overlap Results and Draw Overlap Result Debug Sphere 
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		if (IsValid(OverlapResult.GetActor()))
		{
			FVector DebugLocation = OverlapResult.GetActor()->GetActorLocation(); 
			DebugLocation.Z += 100.f; 
			DrawDebugSphere(World, DebugLocation, 30.f, 10, FColor::Green, false, 3.f); 
		}
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

	if (IsValid(DamageEffectParams.TargetAbilitySystemComponent))
	{
		DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}
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

void UOWAbilitySystemLibrary::AddInteractableActorsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<IInteractInterface>>& OutInteractableActors)
{
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		TScriptInterface<IInteractInterface> InteractableActor(OverlapResult.GetActor()); 
		if (InteractableActor)
		{
			OutInteractableActors.AddUnique(InteractableActor);
		}

		TScriptInterface<IInteractInterface> InteractableComponent(OverlapResult.GetComponent()); 
		if (InteractableComponent)
		{
			OutInteractableActors.AddUnique(InteractableComponent); 
		}
	}
}

void UOWAbilitySystemLibrary::AddInteractableActorsFromHitResult(const FHitResult& HitResult, TArray<TScriptInterface<IInteractInterface>>& OutInteractableActors)
{
	TScriptInterface<IInteractInterface> InteractableActor(HitResult.GetActor());
	if (InteractableActor)
	{
		OutInteractableActors.AddUnique(InteractableActor);
	}
}

AActor* UOWAbilitySystemLibrary::GetActorFromInteractableAgent(TScriptInterface<IInteractInterface> InteractableAgent)
{
	if (UObject* InteractableObject = InteractableAgent.GetObject())
	{
		if (AActor* InteractableActor = Cast<AActor>(InteractableObject))
		{
			return InteractableActor;
		}
		else if (UActorComponent* InteractableActorComponent = Cast<UActorComponent>(InteractableObject))
		{
			return InteractableActorComponent->GetOwner();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Type of Interactable Object is Neither Actor nor Actor Component")); 
			return nullptr; 
		}
	}

	return nullptr;
}
