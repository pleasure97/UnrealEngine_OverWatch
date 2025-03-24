// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacterBase.h"
#include "OWGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/HealthBarPool.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Player/OWPlayerState.h"


AOWCharacterBase::AOWCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	const FOWGameplayTags& OWGameplayTags = FOWGameplayTags::Get();

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon"); 
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket")); 
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("StunDebuffComponent")); 
	StunDebuffComponent->SetupAttachment(GetRootComponent()); 
	StunDebuffComponent->DebuffTag = OWGameplayTags.Debuff_Stun; 

	// Widget Component 
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetOwnerNoSee(true);
	WidgetComponent->SetIsReplicated(true); 
}

UAbilitySystemComponent* AOWCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AOWCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(AOWCharacterBase, bIsStunned);
	DOREPLIFETIME(AOWCharacterBase, bIsBeingShocked); 
	DOREPLIFETIME(AOWCharacterBase, bIsBeingHealed); 
}

UAnimMontage* AOWCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage; 
}

FOnASCRegistered& AOWCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered; 
}

FOnDeath& AOWCharacterBase::GetOnDeathDelegate()
{
	return OnDeath; 
}

void AOWCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true)); 
	MulticastHandleDeath(DeathImpulse); 
}

bool AOWCharacterBase::IsDead_Implementation() const
{
	return bDead; 
}

AActor* AOWCharacterBase::GetAvatar_Implementation()
{
	return this; 
}

bool AOWCharacterBase::IsBeingShocked_Implementation() const
{
	return bIsBeingShocked; 
}

void AOWCharacterBase::SetIsBeingShocked_Implementation(bool bInShock)
{
	bIsBeingShocked = bInShock; 
}

bool AOWCharacterBase::IsBeingHealed_Implementation() const
{
	return bIsBeingHealed; 
}

void AOWCharacterBase::SetIsBeingHealed_Implementation(bool bInHeal)
{
	bIsBeingHealed = bInHeal; 
}

USkeletalMeshComponent* AOWCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

void AOWCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	Weapon->SetSimulatePhysics(true); 
	Weapon->SetEnableGravity(true); 
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); 
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true); 

	GetMesh()->SetEnableGravity(true); 
	GetMesh()->SetSimulatePhysics(true); 
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); 
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); 
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true); 

	bDead = true; 
	OnDeath.Broadcast(this); 
}

void AOWCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//if (!IsLocallyControlled() || GetWorld()->GetNetMode() == NM_DedicatedServer)
	//{
	//	return;
	//}

	if (UHealthBarPool* HealthBarPool = Cast<UHealthBarPool>(WidgetComponent->GetUserWidgetObject()))
	{
		HealthBarPool->SetWidgetController(this); 
		HealthBarPool->BindWidgetControllerEvents(); 
	}
}

FOnAttributeChangedSignature* AOWCharacterBase::GetDelegateForTag(const FGameplayTag& Tag)
{
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
	if (Tag == GameplayTags.Attributes_Defense_Health) return &OnHealthChanged;
	if (Tag == GameplayTags.Attributes_Defense_MaxHealth) return &OnMaxHealthChanged;
	if (Tag == GameplayTags.Attributes_Defense_Armor) return &OnArmorChanged;
	if (Tag == GameplayTags.Attributes_Defense_MaxArmor) return &OnMaxArmorChanged;
	if (Tag == GameplayTags.Attributes_Defense_Shield) return &OnShieldChanged;
	if (Tag == GameplayTags.Attributes_Defense_MaxShield) return &OnMaxShieldChanged;
	if (Tag == GameplayTags.Attributes_Defense_TempArmor) return &OnTempArmorChanged;
	if (Tag == GameplayTags.Attributes_Defense_TempShield) return &OnTempShieldChanged;

	return nullptr;
}

void AOWCharacterBase::BindAttributeChange(UAbilitySystemComponent* ASC, const FGameplayTag& Tag, const FGameplayAttribute& Attribute, FOnAttributeChangedSignature& Delegate)
{
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
		[&](const FOnAttributeChangeData& Data)
		{
			Delegate.Broadcast(Data.NewValue);
		}
	);
}

void AOWCharacterBase::AddHeroAbilities()
{
	UOWAbilitySystemComponent* OWASC = CastChecked<UOWAbilitySystemComponent>(AbilitySystemComponent); 

	if (!HasAuthority()) return; 

	OWASC->AddHeroAbilities(); 
}

void AOWCharacterBase::InitializeDefaultAttributes() const
{
	if (!DefaultVitalAttributes)
	{
		UE_LOG(LogTemp, Log, TEXT("DefaultVitalAttributes is null, skipping attribute initialization."));
		return;
	}
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f); 
}

void AOWCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent())); 
	check(GameplayEffectClass); 

	FGameplayEffectContextHandle GameplayEffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext(); 
	GameplayEffectContextHandle.AddSourceObject(this); 
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, GameplayEffectContextHandle); 
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), GetAbilitySystemComponent()); 
}

void AOWCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0; 
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

