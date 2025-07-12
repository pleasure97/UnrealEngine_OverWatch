// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "Interface/TeamInterface.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "OWCharacterBase.generated.h"


class UOWAbilitySystemComponent;
class UOWAttributeSet;
class UAnimMontage; 
class UGameplayAbility; 
class UDebuffNiagaraComponent; 
class UGameplayEffect; 
class UWidgetComponent;
class UHealthPlateSourceComponent; 

UCLASS(ABSTRACT)
class FPSTEMPLATE_API AOWCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface, public ITeamInterface
{
	GENERATED_BODY()

public:
	AOWCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; 

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const; 

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; 

	/** Combat Interface **/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override; 
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override; 
	virtual FOnDamageSignature& GetOnDamageSignature() override; 
	virtual FOnDeath& GetOnDeathDelegate() override; 
	virtual void Die(const FVector& DeathImpulse) override; 
	virtual bool IsDead_Implementation() const override; 
	virtual AActor* GetAvatar_Implementation() override; 
	virtual bool IsBeingShocked_Implementation() const override;
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override; 
	virtual bool IsBeingHealed_Implementation() const override;
	virtual void SetIsBeingHealed_Implementation(bool bInHeal) override; 
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override; 

	FOnASCRegistered OnASCRegistered; 
	FOnDamageSignature OnDamage; 
	FOnDeath OnDeath; 
	/** Combat Interface End **/

	/** Pawn Interface **/
	virtual void NotifyControllerChanged() override; 
	/** Pawn Interface End **/

	/** Team Interface **/
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override; 
	virtual FGenericTeamId GetGenericTeamId() const override; 
	virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override; 
	/** Team Interface End **/

	/** Attribute Value Changed Delegate **/
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxArmorChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxShieldChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnArmorChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnShieldChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnTempArmorChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnTempShieldChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnOverHealthChanged;

	FOnAttributeChangedSignature* GetDelegateForTag(const FGameplayTag& Tag); 

	/** Attribute Value Changed Delegate End **/

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse); 

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsStunned = false;

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override; 
	virtual void UnPossessed() override; 
	
	void AddHeroAbilities(); 

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon; 

	UPROPERTY()
	TObjectPtr<UOWAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UOWAttributeSet> AttributeSet; 

	/* Attributes */
	virtual void InitializeDefaultAttributes() const; 

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const; 

	/* State - Death */
	UPROPERTY()
	bool bDead = false; 

	/* State - Stun */
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount); 

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 550.f; 

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;

	/* State - Heal */
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingHealed = false;

	/* Widget Component */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY()
	TObjectPtr<UHealthPlateSourceComponent> HealthPlateSourceComponent; 

	/* Team */
	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	virtual FGenericTeamId DetermineNewTeamAfterPossessionEnds(FGenericTeamId OldTeamId) const
	{
		return FGenericTeamId::NoTeam; 
	}

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate; 

	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID); 

	void UpdateTeamColor(); 
};
