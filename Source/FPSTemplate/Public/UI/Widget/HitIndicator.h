// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "HitIndicator.generated.h"

class UBorder; 
class UMaterialInstanceDynamic; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHitIndicator : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* StartAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* EndAnimation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HitIndicator; 

	UPROPERTY()
	UMaterialInstanceDynamic* MID_HitIndicator;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float MaxDamage = 120.f;

	UFUNCTION()
	void OnStartAnimationFinished(); 

	UFUNCTION()
	void OnEndAnimationFinished();

	void ClearHitIndicator();

	float CalculateHitDirectionNormalized(AActor* InOwnerActor, const FVector& InDamageCauserLocation);

	void SetHitInfo(AActor* InDamageCauser, AActor* InOwnerActor, float InDamage);

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	TObjectPtr<AActor> OwnerActor = nullptr; 
	TObjectPtr<AActor> DamageCauser = nullptr; 
	float Damage = 0.f; 

	FVector OwnerActorForwardVector; 
	FVector OwnerActorLocation; 
	FVector DamageCauserLocation; 

	FWidgetAnimationDynamicEvent OnStartAnimationFinishedDelegate; 
	FWidgetAnimationDynamicEvent OnEndAnimationFinishedDelegate; 
};
