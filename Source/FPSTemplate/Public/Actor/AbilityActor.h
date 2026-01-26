// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Interface/TeamInterface.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilityActor.generated.h"

class UGameplayEffect;

UCLASS()
class FPSTEMPLATE_API AAbilityActor : public AActor, public IAbilitySystemInterface, public ITeamInterface
{
	GENERATED_BODY()
	
public:	
	AAbilityActor();

	/* Team */
	virtual FGenericTeamId GetGenericTeamId() const override;

	UFUNCTION(BlueprintCallable)
	void SetTeamID(const int32 InTeamID);

	UFUNCTION(BlueprintCallable)
	void InitializeTeamID();

	/* Vital Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> VitalAttributes;

	/* Ability System */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	UOWAttributeSet* GetAttributeSet() const;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitAbilityActorInfo();

	virtual void InitializeVitalAttributes(); 

	/* Ability System */
	UPROPERTY()
	TObjectPtr<UOWAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UOWAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float Level = 1.f;

	/* Team */
	UPROPERTY(Replicated, EditAnywhere, Category = "Team")
	int32 TeamID = INDEX_NONE;
};
