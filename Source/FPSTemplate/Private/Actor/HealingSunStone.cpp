// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/HealingSunStone.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widget/HealthPlate.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OWGameplayTags.h"

AHealingSunStone::AHealingSunStone()
{
	// Replication 
	SetReplicatingMovement(true); 

	// Box Component (Root Component)
	Box = CreateDefaultSubobject<UBoxComponent>("Box"); 
	Box->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics); 
	Box->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName); 
	Box->SetSimulatePhysics(true); 
	Box->SetNotifyRigidBodyCollision(true); 
	Box->OnComponentHit.AddDynamic(this, &AHealingSunStone::OnAttached); 
	SetRootComponent(Box); 

	// Sun Stone (Static Mesh Component)
	SunStone = CreateDefaultSubobject<UStaticMeshComponent>("SunStone");
	SunStone->SetupAttachment(GetRootComponent());

	// Pedestal (Static Mesh Component)
	Pedestal = CreateDefaultSubobject<UStaticMeshComponent>("Pedestal"); 
	Pedestal->SetupAttachment(GetRootComponent()); 

	// Sun Ray (Niagara Component)
	SunRay = CreateDefaultSubobject<UNiagaraComponent>("SunRay");
	SunRay->SetupAttachment(SunStone);
	SunRay->Deactivate();
	
	// Healing Ray (Niagara Component)
	HealingRay = CreateDefaultSubobject<UNiagaraComponent>("HealingRay"); 
	HealingRay->SetupAttachment(SunStone);
	HealingRay->bAutoActivate = false;

	// Health Plate Widget Component 
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent"); 
	WidgetComponent->SetupAttachment(GetRootComponent()); 
	WidgetComponent->SetIsReplicated(true); 
}

void AHealingSunStone::BeginPlay()
{
	Super::BeginPlay(); 

	InitializeHealthPlate(); 
}

void AHealingSunStone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(AttributeSet))
	{
		AttributeSet->OnDeath.RemoveAll(this); 
	}

	Super::EndPlay(EndPlayReason);
}

void AHealingSunStone::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	if (HasAuthority())
	{
		InitializeHealingAbility();
	}

	AttributeSet->OnDeath.AddUObject(this, &AHealingSunStone::OnDestroyed); 
}

void AHealingSunStone::InitializeHealingAbility()
{
	FGameplayAbilitySpec HealingSunStoneAbilitySpec = FGameplayAbilitySpec(HealingSunStoneAbilityClass, 1 /*int32 InLevel*/);
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpecHandle HealingSunStoneGameplayAbilitySpecHandle = 
			AbilitySystemComponent->GiveAbilityAndActivateOnce(HealingSunStoneAbilitySpec);
		bool bActivated = AbilitySystemComponent->TryActivateAbility(HealingSunStoneGameplayAbilitySpecHandle);
		if (bActivated)
		{
			UE_LOG(LogTemp, Log, TEXT("GA_HealingSunStone is Activated")); 
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("GA_HealingSunStone is NOT Activated"));
		}
	}
}

void AHealingSunStone::InitializeHealthPlate()
{
	if (WidgetComponent)
	{
		if (UHealthPlate* HealthPlate = Cast<UHealthPlate>(WidgetComponent->GetUserWidgetObject()))
		{
			HealthPlate->SetAbilitySystemComponent(AbilitySystemComponent); 
		}
	}
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
	if (Hit.GetActor() == nullptr || Hit.GetActor() == this || !Hit.bBlockingHit)
	{
		return;
	}

	// Collision Object Type
	ECollisionChannel CollisionChannel = Hit.GetComponent()->GetCollisionObjectType(); 
	
	// WorldStatic, WorldDynamic, Vehicle Only
	if (CollisionChannel == ECollisionChannel::ECC_WorldStatic ||
		CollisionChannel == ECollisionChannel::ECC_WorldDynamic ||
		CollisionChannel == ECollisionChannel::ECC_Vehicle)
	{
		// Disable Physics
		Box->SetSimulatePhysics(false);
		Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Box->SetAllPhysicsLinearVelocity(FVector::ZeroVector);

		// Setup Attachment Location
		const FVector AttachmentLocation = Hit.ImpactPoint + Box->GetScaledBoxExtent() * Hit.ImpactNormal;
		const FRotator AttachmentRotation = UKismetMathLibrary::MakeRotFromZ(Hit.ImpactNormal);

		SetActorLocationAndRotation(AttachmentLocation, AttachmentRotation, false, nullptr, ETeleportType::TeleportPhysics);

		// Attach
		AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform);

		if (SunRay)
		{
			SunRay->Activate();
		}
	}
}

void AHealingSunStone::OnDestroyed(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude)
{
	if (HasAuthority())
	{
		FGameplayEventData Payload;
		FGameplayEffectContextHandle GameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		GameplayEffectContextHandle.AddOrigin(GetActorLocation());
		if ((AttributeSet->GetShield() < AttributeSet->GetMaxShield()) || (AttributeSet->GetHealth() < AttributeSet->GetMaxHealth()))
		{
			Payload.EventMagnitude = CooldownWhenDamaged;
		}
		else
		{
			Payload.EventMagnitude = CooldownWhenNotDamaged;
		}
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwner(), FOWGameplayTags::Get().Event_Illiari_PylonDestroyed_Unintentional, Payload);

		Destroy(); 
	}
}
