// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacterBase.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystemBlueprintLibrary.h"


AOWCharacterBase::AOWCharacterBase()
{
	bReplicates = true; 
	SetReplicateMovement(true);

	PrimaryActorTick.bCanEverTick = false;
	const FOWGameplayTags& OWGameplayTags = FOWGameplayTags::Get();

	// Stun Debuff Component
	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("StunDebuffComponent")); 
	StunDebuffComponent->SetupAttachment(GetRootComponent()); 
	StunDebuffComponent->DebuffTag = OWGameplayTags.Debuff_Stun; 

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AOWCharacterBase::OnWallHit);
}

UAbilitySystemComponent* AOWCharacterBase::GetAbilitySystemComponent() const
{
	return nullptr;
}

UOWAttributeSet* AOWCharacterBase::GetAttributeSet() const
{
	return nullptr;
}

void AOWCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(AOWCharacterBase, MyTeamID); 

	DOREPLIFETIME(AOWCharacterBase, bIsStunned);
	DOREPLIFETIME(AOWCharacterBase, bIsBeingShocked); 
	DOREPLIFETIME(AOWCharacterBase, bIsBeingHealed); 
}

float AOWCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 
	OnDamage.Broadcast(DamageTaken); 

	return DamageTaken;
}

UAnimMontage* AOWCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage; 
}

FOnASCRegistered& AOWCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered; 
}

FOnDamageSignature& AOWCharacterBase::GetOnDamageSignature()
{
	return OnDamage; 
}

FOnDeath& AOWCharacterBase::GetOnDeathDelegate()
{
	return OnDeath; 
}

void AOWCharacterBase::Die(const FVector& DeathImpulse)
{
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

void AOWCharacterBase::NotifyControllerChanged()
{
	const FGenericTeamId OldTeamID = GetGenericTeamId(); 

	Super::NotifyControllerChanged(); 

	// Update our Team ID based on the controller
	if (HasAuthority() && (Controller != nullptr))
	{
		if (ITeamInterface* ControllerWithTeamInterface = Cast<ITeamInterface>(Controller))
		{
			MyTeamID = ControllerWithTeamInterface->GetGenericTeamId(); 
			BroadcastTeamChanged(this, OldTeamID, MyTeamID); 
		}
	}
}

void AOWCharacterBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (GetController() == nullptr)
	{
		if (HasAuthority())
		{
			const FGenericTeamId OldTeamID = MyTeamID; 
			MyTeamID = NewTeamID; 
			BroadcastTeamChanged(this, OldTeamID, NewTeamID); 
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a character (%s) except on the authority."), *GetPathName(this)); 
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a possessed character (%s); it's driven by the associated controller"), *GetPathNameSafe(this));
	}
}

FGenericTeamId AOWCharacterBase::GetGenericTeamId() const
{
	return MyTeamID; 
}

FOnTeamIndexChangedDelegate* AOWCharacterBase::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate; 
}

int32 AOWCharacterBase::GetTeamID() const
{
	return GenericTeamIdToInteger(MyTeamID);
}

void AOWCharacterBase::UnPossessed()
{
	// Save Old Controller and Team ID 
	AController* OldController = Controller; 
	const FGenericTeamId OldTeamID = MyTeamID; 
	// Remove Team Changed Delegate's Bindings 
	if (ITeamInterface* ControllerWithTeamInterface = Cast<ITeamInterface>(OldController))
	{
		ControllerWithTeamInterface->GetTeamChangedDelegate().RemoveAll(this); 
	}

	Super::UnPossessed(); 

	// Broadcast that the Team has Changed 
	// TODO - Maintain Team ID 
	MyTeamID = DetermineNewTeamAfterPossessionEnds(OldTeamID);
	BroadcastTeamChanged(this, OldTeamID, MyTeamID); 
	UpdateTeamColor();
}

void AOWCharacterBase::AddHeroAbilities()
{
	UOWAbilitySystemComponent* OWASC = CastChecked<UOWAbilitySystemComponent>(GetAbilitySystemComponent()); 

	if (!HasAuthority()) return; 

	OWASC->AddHeroAbilities(); 
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

void AOWCharacterBase::OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Check if Character Has 'Status_Suppressed' GameplayTag
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
	if (!GetAbilitySystemComponent()->HasMatchingGameplayTag(GameplayTags.Status_Suppressed))
	{
		return;
	}

	if (!IsValid(OtherComp))
	{
		return;
	}

	if (OtherComp->GetCollisionObjectType() != ECollisionChannel::ECC_WorldStatic)
	{
		return;
	}

	if (HasAuthority())
	{
		FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, GameplayTags.Debuff_Suppression_WallHit, Payload);
	}
}

void AOWCharacterBase::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	BroadcastTeamChanged(this, OldTeamID, MyTeamID); 
	// Highlight the Outline of Character's Mesh depending on team (e.g., Blue or Red)
	UpdateTeamColor();
}

void AOWCharacterBase::UpdateTeamColor()
{
	const int32 TeamID = GenericTeamIdToInteger(MyTeamID);
	if (GetMesh())
	{
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(TeamID);
	}
}