// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DS_MatchPlayerState.h"
#include "AbilitySystemInterface.h"
#include "Interface/TeamInterface.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "OWPlayerState.generated.h"

class UOWAbilitySystemComponent; 
class UAttributeSet; 

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, int32 /* Stat Value */)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*Stat Value*/, bool /*bLevelUp*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHeroNameChanged, AOWPlayerState*, /*OWPlayerState */ EHeroName /*HeroName*/)

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWPlayerState : public ADS_MatchPlayerState, public IAbilitySystemInterface, public ITeamInterface 
{
	GENERATED_BODY()

public:
	AOWPlayerState();

	/* Ability System */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet;  }

	/* Hero Info */
	UPROPERTY(ReplicatedUsing = OnRep_HeroName, BlueprintReadOnly)
	EHeroName HeroName;

	EHeroName GetHeroName() const { return HeroName; }
	void SetHeroName(EHeroName NewHeroName);

	UFUNCTION()
	void OnRep_HeroName();

	/* LevelUp Info */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo; 

	/* Delegates */
	FOnPlayerStateChanged OnXPChangedDelegate;
	FOnLevelChanged OnLevelChangedDelegate; 
	FOnPlayerStateChanged OnAttributePointsChangedDelegate; 
	FOnPlayerStateChanged OnSpellPointsChangedDelegate; 
	FOnHeroNameChanged OnHeroNameChangedDelegate; 

	/* Level Up Info */ 
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	void AddToXP(int32 InXP); 
	void AddToLevel(int32 InLevel); 
	void AddToAttributePoints(int32 InAttributePoints); 
	void AddToSpellPoints(int32 InSpellPoints); 

	void SetXP(int32 InXP); 
	void SetLevel(int32 InLevel); 
	void SetAttributePoints(int32 InAttributePoints); 
	void SetSpellPoints(int32 InSpellPoints); 

	/* Team Interface */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override; 
	virtual FGenericTeamId GetGenericTeamId() const override; 
	virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override; 

	UFUNCTION()
	int32 GetTeamId() const { return GenericTeamIdToInteger(MyTeamID);  }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override; 
protected:
	UPROPERTY()
	TObjectPtr<UOWAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; 

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1; 

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0; 

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0; 

	UPROPERTY(VisibleAnywhere, ReplicatedUsing =  OnRep_SpellPoints)
	int32 SpellPoints = 0; 

private:
	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel); 

	UFUNCTION()
	void OnRep_XP(int32 OldXP); 

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints); 

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints); 

	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID); 
};
