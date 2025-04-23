// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EnergyBarrier.generated.h"

class UBoxComponent; 
class UStaticMeshComponent; 

UCLASS()
class FPSTEMPLATE_API AEnergyBarrier : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AEnergyBarrier();

	/* Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> BarrierField; 

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnShieldChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxShieldChanged;

	

	/* Ability System Interface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	void InitAbilityActorInfo(); 

	void InitializeVitalAttributes();

	/* Vital Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> VitalAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float Level = 1.f;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
