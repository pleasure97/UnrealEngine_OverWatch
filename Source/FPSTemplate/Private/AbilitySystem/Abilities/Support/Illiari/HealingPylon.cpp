// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/HealingPylon.h"
#include "Actor/HealingSunStone.h"
#include "OWGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/Widget/OWUserWidget.h"

void UHealingPylon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	const FVector ActorLocation = ActorInfo->AvatarActor->GetRootComponent()->GetComponentLocation(); 

	// Spawn Healing SunStone
	FTransform SpawnTransform; 
	SpawnTransform.SetLocation(ActorLocation + PylonRelativeLocation); 
	HealingSunStone = GetWorld()->SpawnActorDeferred<AHealingSunStone>(
		HealingSunStoneClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		CurrentActorInfo->PlayerController->GetPawn(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn); 

	// Bind OnDestroyed
	HealingSunStone->OnDestroyed.AddDynamic(this, &UHealingPylon::OnHealingSunStoneDestroyed);

	// Bind OnDamaged 
	UAbilitySystemComponent* HealingSunStoneASC = HealingSunStone->GetAbilitySystemComponent();
	if (!HealingSunStoneASC) return;

	HealingSunStoneASC->GetGameplayAttributeValueChangeDelegate(
		UOWAttributeSet::GetHealthAttribute()).AddUObject(this, &UHealingPylon::OnHealingSunStoneDamaged);

	HealingSunStoneASC->GetGameplayAttributeValueChangeDelegate(
		UOWAttributeSet::GetShieldAttribute()).AddUObject(this, &UHealingPylon::OnHealingSunStoneDamaged);

	HealingSunStone->SetOwner(GetAvatarActorFromActorInfo()); 

	HealingSunStone->FinishSpawning(SpawnTransform); 

	// Throw Healing SunStone
	FVector ThrowVelocity = PylonRelativeLocation.GetSafeNormal() * PylonThrowingSpeed;
	HealingSunStone->Throw(ThrowVelocity);
}

void UHealingPylon::OnHealingSunStoneDestroyed(AActor* DestroyedActor)
{
	UAbilitySystemComponent* HealingSunStoneASC = HealingSunStone->GetAbilitySystemComponent();
	if (!HealingSunStoneASC) return; 

	bool bHealthAttributeFound = false; 
	float Health = HealingSunStoneASC->GetGameplayAttributeValue(UOWAttributeSet::GetHealthAttribute(), bHealthAttributeFound); 
	if (!bHealthAttributeFound) return; 

	if (Health == 0.f || bSunStoneDamaged)
	{
		ApplyDynamicCooldown(DamagedCooldownTime); 
	}
	else
	{
		ApplyDynamicCooldown(GeneralCooldownTime); 
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UHealingPylon::OnHealingSunStoneDamaged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue >= Data.OldValue) return; 

	if (Data.Attribute == UOWAttributeSet::GetHealthAttribute())
	{
		if (Data.NewValue == 0.f)
		{
			if (GetAvatarActorFromActorInfo()->HasAuthority())
			{
				HealingSunStone->Destroy();
			}

			if (IsLocallyControlled())
			{
				// Set Owner Character
				ACharacter* OwnerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
				// Set Player Controller
				APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());

				// Project World Location to Widget Position 
				FVector2D ScreenPosition;
				UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PlayerController, HealingSunStone->GetActorLocation(), ScreenPosition, false);

				PylonDestroyedWidget = CreateWidget<UOWUserWidget>(PlayerController, PylonDestroyedWidgetClass);
				PylonDestroyedWidget->AddToViewport();
				PylonDestroyedWidget->SetPositionInViewport(ScreenPosition, false);
				GetWorld()->GetTimerManager().SetTimer(RemoveWidgetTimerHandle, this, &UHealingPylon::RemoveWidget, 2.f, false);
			}
		}
	}

	if (DamageCheckTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageCheckTimerHandle); 
		bSunStoneDamaged = true; 
	}
	GetWorld()->GetTimerManager().SetTimer(DamageCheckTimerHandle, this, &UHealingPylon::UpdateSunStoneDamagedState, 3.f, false);
}

void UHealingPylon::UpdateSunStoneDamagedState()
{
	bSunStoneDamaged = false; 
}

void UHealingPylon::ApplyDynamicCooldown(float CooldownTime)
{
	FGameplayEffectSpecHandle CooldownSpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass, GetAbilityLevel());

	CooldownSpecHandle.Data->SetSetByCallerMagnitude(FOWGameplayTags::Get().Data_Cooldown,CooldownTime); 

	ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, CooldownSpecHandle);
}

void UHealingPylon::RemoveWidget()
{
	if (PylonDestroyedWidget)
	{
		PylonDestroyedWidget->RemoveFromParent();
	}
}

