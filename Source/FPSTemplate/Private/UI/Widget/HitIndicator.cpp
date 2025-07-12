// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HitIndicator.h"
#include "Components/Border.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/WidgetAnimation.h"

void UHitIndicator::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (Border_HitIndicator)
	{
		MID_HitIndicator = Border_HitIndicator->GetDynamicMaterial();
	}

	if (StartAnimation)
	{
		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
		AnimationFinishedEvent.BindUFunction(this, FName("OnStartAnimationFinished"));
		BindToAnimationFinished(StartAnimation, AnimationFinishedEvent);
		PlayAnimationForward(StartAnimation); 
	}

	if (EndAnimation)
	{
		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
		AnimationFinishedEvent.BindUFunction(this, FName("OnEndAnimationFinished"));
		BindToAnimationFinished(EndAnimation, AnimationFinishedEvent);
	}
}

void UHitIndicator::SetHitInfo(AActor* InDamageCauser, AActor* InOwnerActor, float InDamage)
{
	DamageCauser = InDamageCauser;
	OwnerActor = InOwnerActor;
	Damage = InDamage;
}

void UHitIndicator::OnStartAnimationFinished()
{
	if (EndAnimation)
	{
		PlayAnimationForward(EndAnimation);
	}
}

void UHitIndicator::OnEndAnimationFinished()
{
	ClearHitIndicator();
}

void UHitIndicator::ClearHitIndicator()
{
	RemoveFromParent();

	if (StartAnimation)
	{
		UnbindAllFromAnimationFinished(StartAnimation);
	}

	if (EndAnimation)
	{
		UnbindAllFromAnimationFinished(EndAnimation);
	}

	if (MID_HitIndicator)
	{
		MID_HitIndicator = nullptr;
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

	if (MID_HitIndicator)
	{
		MID_HitIndicator = nullptr;
	}

	Super::NativeDestruct(); 
}

float UHitIndicator::CalculateHitDirectionNormalized(AActor* InOwnerActor, const FVector& InDamageCauserLocation)
{
	// Save Player's Forward Vector, Player's Location, and Damage Causer's Location 
	OwnerActorForwardVector = InOwnerActor->GetActorForwardVector();
	OwnerActorLocation = InOwnerActor->GetActorLocation();
	DamageCauserLocation = InDamageCauserLocation;

	// Player -> Damage Causer Direction Vector 
	const FVector ToDamage = (OwnerActor->GetActorLocation() - DamageCauserLocation);
	const FVector ToDamageNormalized = UKismetMathLibrary::Normal(ToDamage, 0.001f); 

	// Calculate Clockwise / CounterClockwise Rotation on 2D Surface between Forward and ToDamage
	const float CrossZ = (OwnerActorForwardVector.X * ToDamageNormalized.Y - OwnerActorForwardVector.Y * ToDamageNormalized.X);
	const float DotValue = UKismetMathLibrary::Dot_VectorVector(OwnerActorForwardVector, ToDamageNormalized); 
	const float AngleDegree = UKismetMathLibrary::Atan2(CrossZ, DotValue);

	return AngleDegree; 
}

void UHitIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime); 

	if (!OwnerActor || !DamageCauser)
	{
		return;
	}

	float AngleValue = CalculateHitDirectionNormalized(OwnerActor, DamageCauser->GetActorLocation());
	float IndicatorScale = UKismetMathLibrary::MapRangeClamped(Damage, 0.f, MaxDamage, 0.05f, 0.4f);
	if (MID_HitIndicator)
	{
		MID_HitIndicator->SetScalarParameterValue(FName("Angle"), AngleValue);
		MID_HitIndicator->SetScalarParameterValue(FName("Alpha"), IndicatorScale);
	}
}