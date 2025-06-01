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
	EHorizontalAlignment GetHAlignment() const { return HAlignment; }

	UFUNCTION(BlueprintCallable)
	EVerticalAlignment GetVAlignment() const { return VAlignment; }

	UFUNCTION(BlueprintCallable)
	void SetVAlignment(EVerticalAlignment InVAlignment) { VAlignment = InVAlignment; }

	UFUNCTION(BlueprintCallable)
	void SetAutoRemoveWhenIndicatorComponentIsNull(bool CanAutomaticallyRemove)
	{
		bAutoRemoveWhenIndicatorComponentIsNull = CanAutomaticallyRemove;
	}
	
	bool CanAutomaticallyRemove() const
	{
		return bAutoRemoveWhenIndicatorComponentIsNull && !IsValid(GetSceneComponent()); 
	}

	UFUNCTION(BlueprintCallable)
	bool GetClampToScreen() const { return bClampToScreen; }

	UFUNCTION(BlueprintCallable)
	void SetClampToScreen(bool CanClampToScreen)
	{
		bClampToScreen = CanClampToScreen;
	}

	UFUNCTION(BlueprintCallable)
	bool GetShowClampToScreenArrow() const { return bShowClampToScreenArrow; }

	UFUNCTION(BlueprintCallable)
	void SetShowClampToScreenArrow(bool bValue)
	{
		bShowClampToScreenArrow = bValue;
	}

	UFUNCTION(BlueprintCallable)
	int32 GetPriority() const { return Priority; }

	UFUNCTION(BlueprintCallable)
	TSoftClassPtr<UUserWidget> GetIndicatorClass() { return IndicatorWidgetClass;  }

	UIndicatorManagerComponent* GetIndicatorMangerComponent() const { return IndicatorManagerComponentPtr.Get(); }
	void SetIndicatorManagerComponent(UIndicatorManagerComponent* InIndicatorManagerComponent); 

	UFUNCTION(BlueprintCallable)
	void UnregisterIndicator(); 

public:
	TWeakObjectPtr<UUserWidget> IndicatorWidget; 

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
	TEnumAsByte<EHorizontalAlignment> HAlignment = HAlign_Center; 

	UPROPERTY()
	TEnumAsByte<EVerticalAlignment> VAlignment = VAlign_Bottom; 

	UPROPERTY()
	int32 Priority = 0; 

	UPROPERTY()
	bool bVisible = true; 

	UPROPERTY()
	bool bAutoRemoveWhenIndicatorComponentIsNull = true; 

	UPROPERTY()
	bool bClampToScreen = true; 

	UPROPERTY()
	bool bShowClampToScreenArrow = false; 

	TWeakPtr<SWidget> CanvasHost; 

	friend class SActorCanvas; 
};
