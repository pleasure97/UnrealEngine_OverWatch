// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "HitIndicator.generated.h"

class UBorder; 
class UMaterialInstanceDynamic; 

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitIndicatorEnd, UHitIndicator*); 

enum class EHitIndicatorState : uint8
{
	Idle, 
	Starting, 
	Ending
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHitIndicator : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HitIndicator; 

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MID_HitIndicator;

	FOnHitIndicatorEnd OnHitIndicatorEnd; 

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> StartAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> EndAnimation;
	
	UFUNCTION()
	void OnEndAnimationFinished();

	float CalculateHitDirectionNormalized(AActor* OwnerActor, const FVector& DamageCauserLocation); 

	void PlayStart(AActor* DamageCauser, AActor* OwnerActor, float Damage); 

	void PlayEnd(); 

	EHitIndicatorState GetHitIndicatorState() const { return HitIndicatorState; }

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FTimerHandle EndTimerHandle; 

	EHitIndicatorState HitIndicatorState = EHitIndicatorState::Idle;

private:
	TObjectPtr<AActor> LastOwnerActor = nullptr; 
	TObjectPtr<AActor> LastDamageCauser = nullptr; 

	FVector LastOwnerActorForwardVector; 
	FVector LastOwnerActorLocation; 
	FVector LastDamageCauserLocation; 
};
