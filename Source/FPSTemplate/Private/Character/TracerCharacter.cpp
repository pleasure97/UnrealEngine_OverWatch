// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TracerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Message/OWMessageTypes.h"
#include "AbilitySystem/GameplayAbilityTargetActor/OWGATargetActor_LineTrace.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "NiagaraFunctionLibrary.h"

ATracerCharacter::ATracerCharacter()
{
	PulseBomb = CreateDefaultSubobject<UChildActorComponent>("PulseBomb"); 
	PulseBomb->SetupAttachment(GetRootComponent()); 
}

FVector ATracerCharacter::GetProjectileStartLocation_Implementation() const
{
	return PulseBomb->GetComponentLocation(); 
}

AOWGATargetActor_LineTrace* ATracerCharacter::GetLineTraceTargetActor_Implementation()
{
	if (LineTraceTargetActor)
	{
		return LineTraceTargetActor;
	}

	LineTraceTargetActor = GetWorld()->SpawnActor<AOWGATargetActor_LineTrace>();
	LineTraceTargetActor->SetOwner(this);
	return LineTraceTargetActor;
}

TArray<FRecallState> ATracerCharacter::GetRecallStates() const
{
	return RecallStates; 
}

UNiagaraComponent* ATracerCharacter::GetDashTrailNiagaraComponent() const
{
	return DashTrailNiagaraComponent; 
}

void ATracerCharacter::BeginPlay()
{
	Super::BeginPlay(); 

	// Spawn System Attached - Dash Trail
	DashTrailNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		DashTrailNiagaraSystem, GetMesh(), FName("DashSocket"),
		FVector::ZeroVector, FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset, false, false); 

	// Record Recall State Every 0.1 Seconds 
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RecordRecallStateTimerHandle, this, &ATracerCharacter::RecordRecallState, 0.1f, true); 
	}
}

void ATracerCharacter::RecordRecallState()
{
	if (AttributeSet)
	{
		RecallStates.Add(FRecallState(UGameplayStatics::GetTimeSeconds(this), GetActorLocation(), AttributeSet->GetHealth()));
		if (RecallStates.Num() > 30)
		{
			RecallStates.RemoveAt(0);
		}
	}
	else
	{
		RecallStates.Empty(); 
	}
}
