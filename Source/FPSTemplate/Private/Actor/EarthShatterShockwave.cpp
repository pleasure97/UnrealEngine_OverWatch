// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EarthShatterShockwave.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OWGameplayTags.h"
#include "AbilitySystemInterface.h"
#include "Team/OWTeamSubsystem.h"

AEarthShatterShockwave::AEarthShatterShockwave()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AEarthShatterShockwave::InitializeShockwave(float InMaxDistance, float InExpansionSpeed, float InArcAngle, float InVerticalHeight, float InShockwaveDirectHitDistance, float InShockwaveDirectHitDamage, float InShockwaveBaseDamage, FDamageEffectParams InDamageEffectParams)
{
	MaxDistance = InMaxDistance;
	ExpansionSpeed = InExpansionSpeed;
	ArcAngle = InArcAngle;
	VerticalHeight = InVerticalHeight;
	ShockwaveDirectHitDistance = InShockwaveDirectHitDistance;
	ShockwaveDirectHitDamage = InShockwaveDirectHitDamage;
	ShockwaveBaseDamage = InShockwaveBaseDamage;
	DamageEffectParams = InDamageEffectParams;
}

void AEarthShatterShockwave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		HandleShockwave(DeltaTime, bDebug);
	}
}

void AEarthShatterShockwave::HandleShockwave(float DeltaTime, bool bInDebug)
{
	// Check if World is Valid
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	// Get GameplayTag Singleton Container
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

	if (CurrentRadius >= MaxDistance)
	{
		FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwner(), GameplayTags.Event_Reinhardt_EarthShatterFinished, Payload);

		Destroy();
	}

	float PreviousRadius = CurrentRadius;
	CurrentRadius += ExpansionSpeed * DeltaTime;

	FVector Forward = GetActorForwardVector();
	FVector Origin = GetActorLocation();

	// Draw Lines for Debugging 
	if (bDebug)
	{
		// Draw Lines at Both Ends of Fan 
		FVector LeftEdge = Forward.RotateAngleAxis(-ArcAngle * 0.5f, FVector::UpVector);
		FVector RightEdge = Forward.RotateAngleAxis(ArcAngle * 0.5f, FVector::UpVector);

		// Draw Fan-Shaped Border 
		DrawDebugLine(World, Origin, Origin + LeftEdge * CurrentRadius, FColor::Red, false, 5.f, 0, 2.f);
		DrawDebugLine(World, Origin, Origin + RightEdge * CurrentRadius, FColor::Red, false, 5.f, 0, 2.f);

		// Draw Arc
		FColor DebugLineColor = (CurrentRadius <= ShockwaveDirectHitDistance) ? FColor::Green : FColor::Red;
		DrawDebugLine(World, Origin + LeftEdge * CurrentRadius, Origin + RightEdge * CurrentRadius, DebugLineColor, false, 5.f, 0, 2.f);
	}
	
	// Target Decision Logic
	TArray<AActor*> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Origin, CurrentRadius, ObjectTypes, nullptr, { GetOwner() }, OUT OverlappedActors);

	for (AActor* OverlappedActor : OverlappedActors)
	{
		// Prevent Already Hit Actor 
		if (HitActors.Contains(OverlappedActor))
		{
			continue;
		}

		// Prevent Ally 
		UOWTeamSubsystem* TeamSubsystem = World->GetSubsystem<UOWTeamSubsystem>();
		if (!IsValid(TeamSubsystem) || TeamSubsystem->CompareTeams(OverlappedActor, GetOwner()) == EOWTeamComparison::OnSameTeam)
		{
			continue;
		}

		FVector Location = OverlappedActor->GetActorLocation();
		FVector Direction = (Location - Origin);
		float DistanceSquared = Direction.SizeSquared2D();

		// Check Distance 
		if (DistanceSquared <= FMath::Square(PreviousRadius) || DistanceSquared > FMath::Square(CurrentRadius))
		{
			continue;
		}

		// Check Vertical Height
		if (FMath::Abs(Direction.Z) > VerticalHeight * 0.5f)
		{
			continue;
		}

		// Check Angle 
		Direction.Z = 0.f;
		Direction.Normalize();
		float AngleCosine = FVector::DotProduct(Forward, Direction);
		// Damage and Knockdown Logic 
		if (AngleCosine >= FMath::Cos(FMath::DegreesToRadians(ArcAngle * 0.5f)))
		{
			HitActors.Add(OverlappedActor);

			// Damage
			DamageEffectParams.BaseDamage =
				(CurrentRadius <= ShockwaveDirectHitDistance) ? ShockwaveDirectHitDamage + ShockwaveBaseDamage : ShockwaveDirectHitDamage;
			// Ability System Component 
			if (IAbilitySystemInterface* OverlappedActorWithASC = Cast<IAbilitySystemInterface>(OverlappedActor))
			{
				DamageEffectParams.TargetAbilitySystemComponent = OverlappedActorWithASC->GetAbilitySystemComponent();
			}
			UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

			// Knockdown
			FGameplayEventData Payload;
			Payload.EventTag = GameplayTags.Debuff_Knockdown;
			Payload.EventMagnitude = KnockDownTime;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OverlappedActor, GameplayTags.Debuff_Knockdown, Payload);
		}
	}
}

