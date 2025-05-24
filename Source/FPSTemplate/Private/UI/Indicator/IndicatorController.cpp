// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Indicator/IndicatorController.h"
#include "SceneView.h"
#include "Component/IndicatorManagerComponent.h"


bool FIndicatorProjection::Project(const UIndicatorController& IndicatorController, const FSceneViewProjectionData& InProjectionData, const FVector2f& ScreenSize, FVector& OutScreenPositionWithDepth)
{
	// Check Scene Component is not nullptr 
	if (USceneComponent* Component = IndicatorController.GetSceneComponent())
	{
		// Calculate World Position 
		// TODO - Check TOptional
		FVector WorldLocation;
		WorldLocation = Component->GetComponentLocation(); 
		const FVector ProjectWorldLocation = WorldLocation + IndicatorController.GetWorldPositionOffset(); 

		// Calcaulte Bounding Box Anchor Point 
		// Projection Mode is set to 'Component Bounding Box' 
		FBox IndicatorBox = Component->Bounds.GetBox(); 
		const FVector ProjectBoxPoint = 
			IndicatorBox.GetCenter() + (IndicatorBox.GetSize() * (IndicatorController.GetBoundingBoxAnchor() - FVector(0.5))); 

		// Project World to Screen 
		FVector2D OutScreenSpacePosition; 
		const bool bInFrontOfCamera = ULocalPlayer::GetPixelPoint(InProjectionData, ProjectBoxPoint, OutScreenSpacePosition, &ScreenSize); 

		// Apply Screen Space Offset 
		OutScreenSpacePosition.X += IndicatorController.GetScreenSpaceOffset().X * (bInFrontOfCamera ? 1 : -1); 
		OutScreenSpacePosition.Y += IndicatorController.GetScreenSpaceOffset().Y; 

		// Process Clamping 
		if (!bInFrontOfCamera && FBox2f(FVector2f::Zero(), ScreenSize).IsInside((FVector2f)OutScreenSpacePosition))
		{
			const FVector2f CenterToPosition = (FVector2f(OutScreenSpacePosition) - (ScreenSize / 2).GetSafeNormal()); 
			OutScreenSpacePosition = FVector2D((ScreenSize / 2) + CenterToPosition * ScreenSize); 
		}

		// Output including Depth Information 
		OutScreenPositionWithDepth = FVector(
			OutScreenSpacePosition.X,
			OutScreenSpacePosition.Y,
			FVector::Dist(InProjectionData.ViewOrigin, ProjectWorldLocation)); 

		return true; 
	}

	return false; 
}

void UIndicatorController::SetIndicatorManagerComponent(UIndicatorManagerComponent* InIndicatorManagerComponent)
{
	// Make Sure Indicator Manager Component Pointer has not yet set. 
	if (ensure(IndicatorManagerComponentPtr.IsExplicitlyNull()))
	{
		IndicatorManagerComponentPtr = InIndicatorManagerComponent; 
	}
}

void UIndicatorController::UnregisterIndicator()
{
	if (UIndicatorManagerComponent* IndicatorManagerComponent = IndicatorManagerComponentPtr.Get())
	{
		IndicatorManagerComponent->RemoveIndicator(this); 
	}
}
