// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "Blueprint/UserWidgetPool.h"
#include "HitIndicatorPool.generated.h"

class UOverlay; 
class UHitIndicator; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHitIndicatorPool : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHitIndicator> HitIndicatorClass; 

	/*UPROPERTY(EditDefaultsOnly)
	FVector2D ViewportSize = FVector2D(250.f, 250.f);

	UPROPERTY(EditDefaultsOnly)
	FVector2D ViewportPosition = FVector2D(0.f, 0.f);

	UPROPERTY(EditDefaultsOnly)
	FAnchors ViewportAnchor = FAnchors(0.5f, 0.5f, 0.5f, 0.5f);*/

	UFUNCTION()
	void ProcessDamageReceived(AActor* DamageCauser, AActor* OwnerActor, float Damage); 

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 
};
