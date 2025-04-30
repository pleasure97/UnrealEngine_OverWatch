// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HitIndicator.h"
#include "Components/Border.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"

void UHitIndicator::NativeConstruct()
{
	Super::NativeConstruct(); 
	SetVisibility(ESlateVisibility::Collapsed); 

	if (Border_HitIndicator)
	{
		MID_HitIndicator = Border_HitIndicator->GetDynamicMaterial(); 
	}

	if (StartAnimation)
	{
		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
		AnimationFinishedEvent.BindUFunction(this, FName("OnStartAnimationFinished"));
		BindToAnimationFinished(StartAnimation, AnimationFinishedEvent);
	}

	if (EndAnimation)
	{
		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
		AnimationFinishedEvent.BindUFunction(this, FName("OnEndAnimationFinished"));
		BindToAnimationFinished(EndAnimation, AnimationFinishedEvent);
	}
}

void UHitIndicator::NativeDestruct()
{
	if (StartAnimation)
	{
		UnbindAllFromAnimationFinished(StartAnimation);
	}

	if (EndAnimation)
	{
		UnbindAllFromAnimationFinished(EndAnimation);
	}

	Super::NativeDestruct(); 
}

float UHitIndicator::CalculateHitDirectionNormalized(AActor* OwnerActor, const FVector& DamageCauserLocation)
{
	// Save Player's Forward Vector, Player's Location, and Damage Causer's Location 
	LastOwnerActorForwardVector = OwnerActor->GetActorForwardVector(); 
	LastOwnerActorLocation = OwnerActor->GetActorLocation(); 
	LastDamageCauserLocation = DamageCauserLocation; 

	// Player -> Damage Causer Direction Vector 
	const FVector ToDamage = (DamageCauserLocation - OwnerActor->GetActorLocation()).GetSafeNormal(); 

	// Calculate Clockwise / CounterClockwise Rotation on 2D Surface between Forward and ToDamage
	const float CrossZ = FVector::CrossProduct(LastOwnerActorForwardVector, ToDamage).Z;
	const float DotProduct = FVector::DotProduct(LastOwnerActorForwardVector, ToDamage);

	// Radians to Degrees
	const float AngleDegree = FMath::RadiansToDegrees(FMath::Atan2(CrossZ, DotProduct)); 

	// Map [-180, 180] degrees to [0, 1] range 
	const float Normalized = FMath::GetMappedRangeValueClamped(FVector2D(-180.f, 180.f), FVector2D(1.f, 0.f), AngleDegree); 

	return Normalized; 
}

void UHitIndicator::PlayStart(AActor* DamageCauser, AActor* OwnerActor, float Damage)
{
	if (HitIndicatorState == EHitIndicatorState::Ending) { return; }
	if (Damage <= 0.f) { return; }

	// Save Damage Causer and Owner Actor 
	LastOwnerActor = OwnerActor; 
	LastDamageCauser = DamageCauser; 
	float ClampedDamage = (Damage >= 120.f) ? 0.4f : FMath::Clamp(Damage, 0.05f, 0.4f);
	float AngleValue = CalculateHitDirectionNormalized(LastOwnerActor, LastDamageCauser->GetActorLocation());
	MID_HitIndicator->SetScalarParameterValue(FName("AlphaIntensity"), ClampedDamage);
	MID_HitIndicator->SetScalarParameterValue(FName("Angle"), AngleValue); 

	HitIndicatorState = EHitIndicatorState::Starting; 

	SetVisibility(ESlateVisibility::Visible); 

	if (StartAnimation)
	{
		PlayAnimation(StartAnimation); 
	}
}

void UHitIndicator::OnStartAnimationFinished()
{
	HitIndicatorState = EHitIndicatorState::Progressing;

	GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(
		EndTimerHandle,
		[this]()	// Callback Lambda
		{
			this->PlayEnd();
		},
		2.f,		// Delay Time
		false		// bLoop
	);
}

void UHitIndicator::PlayEnd()
{
	if (HitIndicatorState == EHitIndicatorState::Ending) { return; }

	HitIndicatorState = EHitIndicatorState::Ending; 

	if (StartAnimation)
	{
		StopAnimation(StartAnimation);
	}
	if (EndAnimation)
	{
		PlayAnimation(EndAnimation);
	}
}

void UHitIndicator::Reset()
{
	FlushRenderingCommands();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
	}

	// Stop All Playing Animations
	StopAllAnimations(); 

	// Force to Clean Up Sequence Player
	for (UUMGSequencePlayer* StoppedPlayer : StoppedSequencePlayers)
	{
		ActiveSequencePlayers.RemoveSwap(StoppedPlayer);
		StoppedPlayer->TearDown();
	}

	StoppedSequencePlayers.Empty();

	// Release Slate Source
	ReleaseSlateResources(true); 

	// Unbind EndAnimation Handler and Bind Again 
	if (EndAnimation)
	{
		UnbindAllFromAnimationFinished(EndAnimation); 
		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
		AnimationFinishedEvent.BindUFunction(this, FName("OnEndAnimationFinished"));
		BindToAnimationFinished(EndAnimation, AnimationFinishedEvent);
	}

	// 
	SynchronizeProperties(); 

	// Reset Dynamic Material Instance
	if (Border_HitIndicator)
	{
		MID_HitIndicator = Border_HitIndicator->GetDynamicMaterial();
	}

	// Reset to Idle State
	HitIndicatorState = EHitIndicatorState::Idle; 
	SetVisibility(ESlateVisibility::Collapsed); 

	// Reset Saved Owner Actor and Damage Causer 
	LastOwnerActor = nullptr; 
	LastDamageCauser = nullptr; 
}

void UHitIndicator::OnEndAnimationFinished()
{
	LastOwnerActor = nullptr; 
	LastDamageCauser = nullptr; 

	HitIndicatorState = EHitIndicatorState::Idle; 
	SetVisibility(ESlateVisibility::Collapsed); 

	OnHitIndicatorEnd.Broadcast(this);
}

void UHitIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime); 

	if (HitIndicatorState != EHitIndicatorState::Progressing) { return; }

	if (!LastOwnerActorLocation.Equals(LastOwnerActor->GetActorLocation()) ||
		!LastOwnerActorForwardVector.Equals(LastOwnerActor->GetActorForwardVector()) ||
		!LastDamageCauserLocation.Equals(LastDamageCauser->GetActorLocation()))
	{
		float AngleValue = CalculateHitDirectionNormalized(LastOwnerActor, LastDamageCauser->GetActorLocation());
		MID_HitIndicator->SetScalarParameterValue(FName("Angle"), AngleValue);
	}
}
