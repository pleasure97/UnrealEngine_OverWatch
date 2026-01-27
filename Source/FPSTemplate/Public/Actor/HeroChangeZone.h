// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/EffectActor.h"
#include "HeroChangeZone.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AHeroChangeZone : public AEffectActor
{
	GENERATED_BODY()

public:
	AHeroChangeZone(); 

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Box;


	UFUNCTION()
	void OnHeroChangeZoneOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHeroChangeZoneEndOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);
};
