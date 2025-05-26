// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/HeroChangeZone.h"
#include "GameFramework/PlayerController.h"
#include "Character/OWCharacterBase.h"
#include "AbilitySystemComponent.h"


void AHeroChangeZone::BeginPlay()
{
	Super::BeginPlay(); 

	OnActorBeginOverlap.AddDynamic(this, &AHeroChangeZone::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AHeroChangeZone::OnOverlapEnd); 
}

void AHeroChangeZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!HasAuthority())
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Cast<APawn>(OtherActor)->GetController()))
	{
		if (AOWCharacterBase* Character = Cast<AOWCharacterBase>(PlayerController->GetPawn()))
		{
			if (UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent())
			{
				FGameplayAbilitySpec GameplayAbilitySpec(GameplayAbility_ChangeHero, 1, INDEX_NONE, this); 
				FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = AbilitySystemComponent->GiveAbility(GameplayAbilitySpec); 
				GrantedHandles.Add(PlayerController, GameplayAbilitySpecHandle); 
			}
		}
	}

}

void AHeroChangeZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!HasAuthority())
	{
		return; 
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Cast<APawn>(OtherActor)->GetController()))
	{
		if (AOWCharacterBase* Character = Cast<AOWCharacterBase>(PlayerController->GetPawn()))
		{
			if (FGameplayAbilitySpecHandle* GameplayAbilitySpecHandle = GrantedHandles.Find(PlayerController))
			{
				Character->GetAbilitySystemComponent()->ClearAbility(*GameplayAbilitySpecHandle);
				GrantedHandles.Remove(PlayerController); 
			}
		}
	}
}
