// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacter.h"
#include "TracerCharacter.generated.h"

struct FRecallState; 
class AOWGATargetActor_LineTrace; 
class UNiagaraSystem; 
class UNiagaraComponent; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API ATracerCharacter : public AOWCharacter
{
	GENERATED_BODY()
	
public:
	ATracerCharacter(); 

	virtual FVector GetProjectileStartLocation_Implementation() const override; 
	virtual AOWGATargetActor_LineTrace* GetLineTraceTargetActor_Implementation() override; 

	UFUNCTION(BlueprintCallable)
	TArray<FRecallState> GetRecallStates() const; 

	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* GetDashTrailNiagaraComponent() const; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> DashTrailNiagaraSystem; 

protected:
	virtual void BeginPlay() override; 

	void RecordRecallState(); 

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> PulseBomb; 

	UPROPERTY()
	TArray<FRecallState> RecallStates; 

	UPROPERTY()
	TObjectPtr<AOWGATargetActor_LineTrace> LineTraceTargetActor; 

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> DashTrailNiagaraComponent; 

	FTimerHandle RecordRecallStateTimerHandle; 
};
