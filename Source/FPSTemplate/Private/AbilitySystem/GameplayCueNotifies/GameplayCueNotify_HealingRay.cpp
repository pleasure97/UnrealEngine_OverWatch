// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayCueNotifies/GameplayCueNotify_HealingRay.h"
#include "NiagaraFunctionLibrary.h"
#include "Interface/CombatInterface.h"
#include "Team/OWTeamSubsystem.h"
#include "NiagaraComponent.h"

bool AGameplayCueNotify_HealingRay::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	const AActor* InstigatorActor = Parameters.GetInstigator(); 
	const UObject* SourceObject = Parameters.GetSourceObject();
	const FVector HitLocation = Parameters.Location;
	USceneComponent* TargetAttachComponent = Parameters.TargetAttachComponent.Get();

	UNiagaraComponent* RaySystem = UNiagaraFunctionLibrary::SpawnSystemAttached(
		HealingRay, TargetAttachComponent, FName("BeamSocket"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	RaySystem->SetVectorParameter(FName("BeamStart"), TargetAttachComponent->GetSocketLocation(FName("BeamSocket")));

	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>(); 
	if (SourceObject->Implements<UCombatInterface>() && TeamSubsystem)
	{
		EOWTeamComparison TeamComparison = TeamSubsystem->CompareTeams(InstigatorActor, SourceObject); 
		if (TeamComparison != EOWTeamComparison::OnSameTeam) { return false; }

		RaySystem->SetVectorParameter(FName("BeamEnd"), HitLocation); 
		// TODO - Set Float Parameter of Particle Liftime and LoopDuration, or Edit Gameplay Ability 
	}

	return true; 
}
