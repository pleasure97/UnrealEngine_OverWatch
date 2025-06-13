// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Message/OWMessageTypes.h"
#include "AssaultPoint.generated.h"

class UBoxComponent; 
class APawn; 
class UMatchScoringComponent; 

UCLASS()
class FPSTEMPLATE_API AAssaultPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AAssaultPoint();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Box; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseCaptureRate = 1 / 12.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TwoPeopleMultiplier = 1.5f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ThreePeopleMultiplier = 2.f; 

protected:
	virtual void BeginPlay() override; 

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override; 

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

private:
	UPROPERTY(Replicated)
	int32 NumAttackers = 0; 

	UPROPERTY(Replicated)
	int32 NumDefenders = 0; 

	UPROPERTY(ReplicatedUsing=OnRep_OccupationProgress)
	float OccupationProgress = 0.f; 

	UPROPERTY()
	int32 AssaultPointID = 1; 

	UPROPERTY(Replicated)
	TArray<AActor*> OverlappingPawns; 

	UPROPERTY(ReplicatedUsing=OnRep_OccupationState)
	TEnumAsByte<EOccupationState> OccupationState = EOccupationState::None;

	UPROPERTY()
	TEnumAsByte<EOccupationState> LastOccupationState = EOccupationState::None;

	UPROPERTY()
	TObjectPtr<UMatchScoringComponent> MatchScoringComponent; 

	void UpdateAssaultPoint(int32 NewNumAttackers, int32 NewNumDefenders); 

	UFUNCTION()
	void OnRep_OccupationProgress(); 

	UFUNCTION()
	void OnRep_OccupationState(); 

	void MakeOccupationMessage(); 
};
