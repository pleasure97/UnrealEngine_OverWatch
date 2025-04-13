// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/HealingSunStone.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Team/OWTeamSubsystem.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Interfaces/CombatInterface.h"
#include "UI/Widget/OWUserWidget.h"

AHealingSunStone::AHealingSunStone()
{
	PrimaryActorTick.bCanEverTick = false; 

	Box = CreateDefaultSubobject<UBoxComponent>("Box"); 
	SetRootComponent(Box); 

	Pedestal = CreateDefaultSubobject<UStaticMeshComponent>("Pedestal"); 
	Pedestal->SetupAttachment(GetRootComponent()); 

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar"); 
	HealthBar->SetupAttachment(GetRootComponent()); 

	SunStone = CreateDefaultSubobject<UStaticMeshComponent>("SunStone"); 
	SunStone->SetupAttachment(GetRootComponent()); 

	SunRay = CreateDefaultSubobject<UNiagaraComponent>("SunRay"); 
	SunRay->SetupAttachment(SunStone); 

	// Box->OnComponentHit.AddDynamic(this, &AHealingStone::OnAttached); 

	AbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("AbilitySystemComponent"); 
	AbilitySystemComponent->SetIsReplicated(true); 
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); 

	AttributeSet = CreateDefaultSubobject<UOWAttributeSet>("AttributeSet"); 
}

void AHealingSunStone::BeginPlay()
{
	Super::BeginPlay(); 

	if (UOWUserWidget* OWUserWidget = Cast<UOWUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		OWUserWidget->SetWidgetController(this); 
	}

	if (const UOWAttributeSet* OWAttributeSet = CastChecked<UOWAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OWAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		); 

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OWAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OWAttributeSet->GetShieldAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnShieldChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OWAttributeSet->GetMaxShieldAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxShieldChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(OWAttributeSet->GetHealth()); 
		OnMaxHealthChanged.Broadcast(OWAttributeSet->GetMaxHealth()); 
		OnShieldChanged.Broadcast(OWAttributeSet->GetShield()); 
		OnMaxShieldChanged.Broadcast(OWAttributeSet->GetMaxShield()); 
	}

	FTimerHandle TimerHandle = UKismetSystemLibrary::K2_SetTimer(this, "ActivateHealing", 0.8f, true, false, 2.f);
}

void AHealingSunStone::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this); 
	Cast<UOWAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet(); 

	if (HasAuthority())
	{
		InitializeVitalAttributes(); 
	}

	OnASCRegistered.Broadcast(AbilitySystemComponent); 
}

void AHealingSunStone::InitializeVitalAttributes()
{
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext(); 

	VitalAttributesContextHandle.AddSourceObject(this); 

	const FGameplayEffectSpecHandle VitalAttributeSpecHandle =
		AbilitySystemComponent->MakeOutgoingSpec(VitalAttributes, Level, VitalAttributesContextHandle); 

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get()); 
}

void AHealingSunStone::Throw(FVector NewVelocity)
{
	Box->SetPhysicsLinearVelocity(NewVelocity); 
}

void AHealingSunStone::OnAttached(
	UPrimitiveComponent* HitComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, 
	const FHitResult& Hit)
{
	if (Hit.GetActor() == nullptr) return;

	ECollisionChannel CollisionChannel = Hit.GetComponent()->GetCollisionObjectType(); 
	
	if (CollisionChannel == ECollisionChannel::ECC_WorldStatic ||
		CollisionChannel == ECollisionChannel::ECC_WorldDynamic ||
		CollisionChannel == ECollisionChannel::ECC_Pawn ||
		CollisionChannel == ECollisionChannel::ECC_PhysicsBody ||
		CollisionChannel == ECollisionChannel::ECC_Vehicle)
	{
		const FRotator NewRotation = UKismetMathLibrary::MakeRotFromZ(Hit.ImpactNormal);
		SetActorRotation(NewRotation); 
		AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform); 
	}
}

void AHealingSunStone::ActivateHealing()
{
	FindAlliance(); 

	if (!AllyToHeal) return; 

	// Adjust Heal Laser 
	UNiagaraComponent* HealLaserComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HealLaser, SunStone->GetComponentLocation()); 
	HealLaserComponent->SetVectorParameter(FName("BeamStart"), SunStone->GetComponentLocation()); 
	HealLaserComponent->SetVectorParameter(FName("BeamEnd"), AllyToHeal->GetActorLocation());

	// Before Applying Damage Effect, Set Information
	if (IAbilitySystemInterface* SourceAbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		DamageEffectParams.SourceAbilitySystemComponent = SourceAbilitySystemInterface->GetAbilitySystemComponent(); 
	}
	if (IAbilitySystemInterface* TargetAbilitySystemInterface = Cast<IAbilitySystemInterface>(AllyToHeal))
	{
		DamageEffectParams.TargetAbilitySystemComponent = TargetAbilitySystemInterface->GetAbilitySystemComponent();
	}
	DamageEffectParams.BaseDamage = HealAmount; 
	UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams); 

	// Play Heal Sound 
	UGameplayStatics::PlaySoundAtLocation(this, HealSound, AllyToHeal->GetActorLocation()); 

	// Spawn Healed Effect
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HealCross, AllyToHeal->GetActorLocation()); 

	// Set Timer to Cooldown Heal 
	bCanHeal = false; 
	FTimerHandle TimerHandle = UKismetSystemLibrary::K2_SetTimer(this, "ResetCanHeal", 0.8f, false, false);
}

void AHealingSunStone::FindAlliance()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoredActors;

	// Sphere Trace To First Check Visible Actors 
	bool bFindAlliance = UKismetSystemLibrary::SphereTraceMulti(this,
		GetActorLocation(), GetActorLocation(), HealRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoredActors, EDrawDebugTrace::None, HitResults, true);
	if (!bFindAlliance) { return; }

	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>(); 
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor(); 
		// Check if Visible Actors Have Implemented Ability System Interface
		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(HitActor); 
		if (!AbilitySystemInterface) continue; 
		// Check if Visible Actors Are Not Dead
		if (!HitActor->Implements<UCombatInterface>()) continue; 
		bool bNotDead = !ICombatInterface::Execute_IsDead(HitActor); 
		//  Check if Visible Actors Are on the Same Team 
		EOWTeamComparison TeamComparison = TeamSubsystem->CompareTeams(this, HitActor); 
		if (AbilitySystemInterface && bNotDead && (TeamComparison == EOWTeamComparison::OnSameTeam))
		{
			UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent(); 
			FindAllyWithLowestHealth(ASC);
		}
	}
}

void AHealingSunStone::FindAllyWithLowestHealth(UAbilitySystemComponent* ASC)
{
	if (!ASC) return;

	float Health = ASC->GetNumericAttribute(FGameplayAttribute(UOWAttributeSet::GetHealthAttribute()));
	float MaxHealth = ASC->GetNumericAttribute(FGameplayAttribute(UOWAttributeSet::GetMaxHealthAttribute()));
	float Armor = ASC->GetNumericAttribute(FGameplayAttribute(UOWAttributeSet::GetArmorAttribute()));
	float MaxArmor = ASC->GetNumericAttribute(FGameplayAttribute(UOWAttributeSet::GetMaxArmorAttribute()));
	float Shield = ASC->GetNumericAttribute(FGameplayAttribute(UOWAttributeSet::GetShieldAttribute()));
	float MaxShield = ASC->GetNumericAttribute(FGameplayAttribute(UOWAttributeSet::GetMaxShieldAttribute()));

	float TempHealth = Health + Armor + Shield; 
	float TempTotalHealth = (Health + Armor + Shield) / (MaxHealth + MaxArmor + MaxShield); 

	if (TempHealth == 0.f) return; 

	// Check LowestHP is not set yet. 
	if (LowestHP == -1.f)
	{
		LowestHP = TempHealth; 
	}
	else
	{
		LowestHP = FMath::Min(LowestHP, TempHealth);
	}
	LowestHPPercent = FMath::Min(LowestHPPercent, TempHealth / TempTotalHealth); 

	if (LowestHP == TempHealth && LowestHPPercent == (TempHealth / TempTotalHealth))
	{
		AllyToHeal = ASC->GetOwnerActor(); 
	}
}

void AHealingSunStone::ResetCanHeal()
{
	bCanHeal = true;
}

