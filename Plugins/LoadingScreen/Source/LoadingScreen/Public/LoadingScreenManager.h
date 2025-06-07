// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "LoadingScreenManager.generated.h"

class ILoadingInterface; 
class IInputProcessor; 

/**
 * 
 */
UCLASS()
class LOADINGSCREEN_API ULoadingScreenManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	/* USubsystem Interface */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override; 
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override; 
	/* USubsystem Interface End */

	/* FTickableObjectBase Interface */
	virtual void Tick(float DeltaTime) override; 
	virtual ETickableTickType GetTickableTickType() const override; 
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override; 
	virtual UWorld* GetTickableGameObjectWorld() const override; 
	/* FTickableObjectBase Interface End */

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoadingScreenVisibilityChangedDelegate, bool); 

	void RegisterLoadingProcessor(TScriptInterface<ILoadingInterface> Interface); 
	void UnregisterLoadingProcessor(TScriptInterface<ILoadingInterface> Interface); 
private:
	void HandlePreLoadMap(const FWorldContext& WorldContext, const FString& MapName); 
	void HandlePostLoadMap(UWorld* World); 

	// Returns true if we need to be showing the loading screen
	bool CheckForAnyNeedToShowLoadingScreen(); 

	// Returns true if we want to be showing the loading screen 
	bool ShouldShowLoadingScreen();

	// Determine if we should show or hide the loading screen, called every frame
	void UpdateLoadingScreen(); 

	// Show loading screen. Setup the local screen widget on the viewport
	void ShowLoadingScreen(); 

	// Hide loading screen. The loading screen will be destroyed
	void HideLoadingScreen();

	// Returns true if we are in the initial loading flow before the screen should be used.
	bool IsShowingInitialLoadingScreen(); 

	// Prevent input from being used in-game while the loading screen is visible
	void StartBlockingInput(); 

	// Resume in-game input
	void StopBlockingInput(); 

	// Remove the widget from viewport
	void RemoveWidgetFromViewport(); 

	void ChangePerformanceSettings(bool bEnableLoadingScreen); 

	// Input processor to eat all input while loading screen is shown
	TSharedPtr<IInputProcessor> InputPreProcessor; 

	// True when we are between PreLoadMap and PostLoadMap
	bool bCurrentlyInLoadMap = false;

	// True when the loading screen is currently being shown
	bool bCurrentlyShowingLoadingScreen = false; 

	// The time the loading screen most recently wanted to be dismissed
	double TimeLoadingScreenLastDismissed = -1.0; 

	// The time when we started showing the loading screen 
	double TimeLoadingScreenShown = 0.0; 
	
	// The time until the next log for why the loading screen is still up 
	double TimeUntilNextLogHeartbeatSeconds = 0.0; 

	// The reason why the loading screen is up or not 
	FString DebugReasonForShowingOrHidingLoadingScreen; 

	// External Loading Processors 
	TArray<TWeakInterfacePtr<ILoadingInterface>> ExternalLoadingProcessors; 

	// Delegate broadcast when the loading screen visibility changes 
	FOnLoadingScreenVisibilityChangedDelegate LoadingScreenVisibilityChanged;

	// A reference to the loading screen widget we are displaying
	TSharedPtr<SWidget> LoadingScreenWidget; 
};
