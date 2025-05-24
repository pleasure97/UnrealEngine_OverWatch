// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IndicatorController.generated.h"

struct FSceneViewProjectionData; 
class UIndicatorManagerComponent; 

struct FIndicatorProjection
{
	bool Project(
		const UIndicatorController& IndicatorController, 
		const FSceneViewProjectionData& InProjectionData, 
		const FVector2f& ScreenSize, 
		FVector& OutScreenPositionWithDepth); 
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UIndicatorController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	UObject* GetDataObject() const { return DataObject; }

	UFUNCTION(BlueprintCallable)
	void SetDataObject(UObject* InDataObject) { DataObject = InDataObject; }

	UFUNCTION(BlueprintCallable)
	USceneComponent* GetSceneComponent() const { return Component; }

	UFUNCTION(BlueprintCallable)
	void SetSceneComponent(USceneComponent* InSceneComponent) { Component = InSceneComponent; }

	UFUNCTION(BlueprintCallable)
	bool GetIsVisible() const { return IsValid(GetSceneComponent()) && bVisible; }

	UFUNCTION(BlueprintCallable)
	void SetDesiredVisibility(bool InVisible)
	{
		bVisible = InVisible; 
	}

	UFUNCTION(BlueprintCallable)
	void SetIndicatorClass(TSoftClassPtr<UUserWidget> InIndicatorWidgetClass) { IndicatorWidgetClass = InIndicatorWidgetClass; }

	UFUNCTION(BlueprintCallable)
	FVector GetBoundingBoxAnchor() const { return BoundingBoxAnchor; }

	UFUNCTION(BlueprintCallable)
	void SetBoundingBoxAnchor(FVector InBoundingBoxAnchor) { BoundingBoxAnchor = InBoundingBoxAnchor; }

	UFUNCTION(BlueprintCallable)
	FVector GetWorldPositionOffset() const { return WorldPositionOffset; }

	UFUNCTION(BlueprintCallable)
	void SetWorldPositionOffset(FVector InWorldPositionOffset) { WorldPositionOffset = InWorldPositionOffset; }

	UFUNCTION(BlueprintCallable)
	FVector2D GetScreenSpaceOffset() const { return ScreenSpaceOffset; }
	
	UFUNCTION(BlueprintCallable)
	void SetScreenSpaceOffset(FVector2D InScreenSpaceOffset) { ScreenSpaceOffset = InScreenSpaceOffset; }

	UFUNCTION(BlueprintCallable)
	void SetVAlignment(EVerticalAlignment InVAlignment) { VAlignment = InVAlignment; }

	UFUNCTION(BlueprintCallable)
	void SetAutoRemoveWhenIndicatorComponentIsNull(bool CanAutomaticallyRemove)
	{
		bAutoRemoveWhenIndicatorComponentIsNull = CanAutomaticallyRemove;
	}

	UFUNCTION(BlueprintCallable)
	void SetClampToScreen(bool CanClampToScreen)
	{
		bClampToScreen = CanClampToScreen;
	}

	UIndicatorManagerComponent* GetIndicatorMangerComponent() const { return IndicatorManagerComponentPtr.Get(); }
	void SetIndicatorManagerComponent(UIndicatorManagerComponent* InIndicatorManagerComponent); 

	UFUNCTION(BlueprintCallable)
	void UnregisterIndicator(); 

private:
	UPROPERTY()
	TObjectPtr<UObject> DataObject; 

	UPROPERTY()
	TObjectPtr<USceneComponent> Component; 

	UPROPERTY()
	TSoftClassPtr<UUserWidget> IndicatorWidgetClass; 

	UPROPERTY()
	TWeakObjectPtr<UIndicatorManagerComponent> IndicatorManagerComponentPtr; 

	UPROPERTY()
	FVector BoundingBoxAnchor = FVector(0.5, 0.5, 1.); 

	UPROPERTY()
	FVector WorldPositionOffset = FVector::ZeroVector;

	UPROPERTY()
	FVector2D ScreenSpaceOffset = FVector2D(0, 0);

	UPROPERTY()
	TEnumAsByte<EVerticalAlignment> VAlignment = VAlign_Bottom; 

	UPROPERTY()
	bool bVisible = true; 

	UPROPERTY()
	bool bAutoRemoveWhenIndicatorComponentIsNull = true; 

	UPROPERTY()
	bool bClampToScreen = true; 
};
