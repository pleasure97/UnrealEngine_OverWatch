// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/AutoReload.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"

bool UAutoReload::CheckBulletsState()
{
	check(CurrentActorInfo); 
	
	if (CurrentActorInfo->AbilitySystemComponent.IsValid())
	{
		float NumCurrentBullets = CurrentActorInfo->AbilitySystemComponent->GetNumericAttribute(UOWAttributeSet::GetNumCurrentBulletsAttribute());
		return NumCurrentBullets <= 0.f;
	}

	return false;
}

void UAutoReload::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec); 

	TryActivateAbilityOnSpawn(ActorInfo, Spec); 
}

void UAutoReload::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting); 

	if (ActorInfo && !Spec.IsActive() && !bIsPredicting)
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get(); 
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get(); 

		// Don't Activate if Avatar Actor is torn off or about to die
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.f))
		{
			// Check if Ability has Local Net Execution Policy - Local Predicted or Local Only 
			const bool bIsLocalExeuction = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			// Check if Ability has Server Net Execution Policy - Server Initiated or Local Only 
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetSecurityPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			// Check Actor Authority 
			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExeuction;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle); 
			}
		}
	}
}
