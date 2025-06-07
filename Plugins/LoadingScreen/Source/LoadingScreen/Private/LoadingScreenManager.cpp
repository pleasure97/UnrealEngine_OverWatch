// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenManager.h"
#include "LoadingInterface.h"
#include "LoadingScreenSettings.h"
#include "GameFramework/GameStateBase.h"
#include "HAL/ThreadHeartbeat.h"
#include "PreLoadScreenManager.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Images/SThrobber.h"
#include "ShaderPipelineCache.h"

/** Loading Interface **/
bool ILoadingInterface::ShouldShowLoadingScreen(UObject* TestObject, FString& OutReason)
{
	if (TestObject != nullptr)
	{
		if (ILoadingInterface* LoadObserver = Cast<ILoadingInterface>(TestObject))
		{
			FString ObserverReason; 
			if (LoadObserver->ShouldShowLoadingScreen(ObserverReason))
			{
				if (ensureMsgf(!ObserverReason.IsEmpty(), TEXT("%s failed to set a reason why it wants to show the loading screen"), *GetPathNameSafe(TestObject)))
				{
					OutReason = ObserverReason; 
				}
				return true; 
			}
		}
	}

	return false; 
}
/** Loading Interface End **/

/** Loading Screen Console Variables **/
namespace LoadingScreenCVars
{
	// Console Variables
	static bool LogLoadingScreenReasonEveryFrame = false;
	static FAutoConsoleVariableRef CVarLogLoadingScreenReasonEveryFrame(
		TEXT("LoadingScreen.LogLoadingScreenReasonEveryFrame"),
		LogLoadingScreenReasonEveryFrame,
		TEXT("When true, the reason the loading screen is shown or hidden will be printed to the log every frame."),
		ECVF_Default); 

	static bool ForceLoadingScreenVisible = false; 
	static FAutoConsoleVariableRef CVarForceLoadingScreenVisible(
		TEXT("LoadingScreen.AlwaysShow"),
		ForceLoadingScreenVisible,
		TEXT("Force the loading screen to show."),
		ECVF_Default); 

	static float HoldAdditionalSeconds = 2.f; 
	static FAutoConsoleVariableRef CVarHoldAdditionalSeconds(
		TEXT("LoadingScreen.HoldAdditionalSeconds"),
		HoldAdditionalSeconds,
		TEXT("How long to hold the loading screen up after other loading finishes to try to give texture streaming a chance to avoid blurriness"),
		ECVF_Default | ECVF_Preview); 
}
/** Loading Screen Console Variables End **/

/** Loading Screen Input PreProcessor **/
class FLoadingScreenInputPreProcessor : public IInputProcessor
{
public:
	FLoadingScreenInputPreProcessor() {}
	virtual ~FLoadingScreenInputPreProcessor() {}

	bool CanEatInput() const
	{
		return !GIsEditor;
	}

	// IInputProcess interface
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override { return CanEatInput(); }
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override { return CanEatInput(); }
	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override { return CanEatInput(); }
	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override { return CanEatInput(); }
	virtual bool HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent) override { return CanEatInput(); }
	// IInputProcess Interface End
};
/** Loading Screen Input PreProcessor End**/

/** Loading Screen Manager **/
void ULoadingScreenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ULoadingScreenManager::HandlePreLoadMap); 
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULoadingScreenManager::HandlePostLoadMap); 

	const UGameInstance* LocalGameInstance = GetGameInstance(); 
	check(LocalGameInstance); 
}

void ULoadingScreenManager::Deinitialize()
{
	StopBlockingInput(); 

	RemoveWidgetFromViewport(); 

	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this); 
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this); 
}

bool ULoadingScreenManager::ShouldCreateSubsystem(UObject* Outer) const 
{
	const UGameInstance* GameInstance = CastChecked<UGameInstance>(Outer); 
	const bool bIsServerWorld = GameInstance->IsDedicatedServerInstance(); 

	return !bIsServerWorld; 
}

void ULoadingScreenManager::Tick(float DeltaTime)
{
	UpdateLoadingScreen(); 

	TimeUntilNextLogHeartbeatSeconds = FMath::Max(TimeUntilNextLogHeartbeatSeconds - DeltaTime, 0.0); 
}

ETickableTickType ULoadingScreenManager::GetTickableTickType() const
{
	return ETickableTickType::Conditional; 
}

bool ULoadingScreenManager::IsTickable() const
{
	return !HasAnyFlags(RF_ClassDefaultObject); 
}
TStatId ULoadingScreenManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULoadingScreenManager, STATGROUP_Tickables); 
}

UWorld* ULoadingScreenManager::GetTickableGameObjectWorld() const
{
	return GetGameInstance()->GetWorld(); 
}

void ULoadingScreenManager::RegisterLoadingProcessor(TScriptInterface<ILoadingInterface> Interface)
{
	ExternalLoadingProcessors.Add(Interface.GetObject()); 
}

void ULoadingScreenManager::UnregisterLoadingProcessor(TScriptInterface<ILoadingInterface> Interface)
{
	ExternalLoadingProcessors.Remove(Interface.GetObject()); 
}

void ULoadingScreenManager::HandlePreLoadMap(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance == GetGameInstance())
	{
		bCurrentlyInLoadMap = true; 

		// Update the Loading Screen Immediately if the Engine is Initialized
		if (GEngine->IsInitialized())
		{
			UpdateLoadingScreen(); 
		}
	}
}

void ULoadingScreenManager::HandlePostLoadMap(UWorld* World)
{
	if ((World != nullptr) && (World->GetGameInstance() == GetGameInstance()))
	{
		bCurrentlyInLoadMap = false;
	}
}

bool ULoadingScreenManager::CheckForAnyNeedToShowLoadingScreen()
{
	// Start out with 'Unknown' reason in case someone forgets to put a reason when changing this in the future. 
	DebugReasonForShowingOrHidingLoadingScreen = TEXT("Reason for Showing/Hiding LoadingScreen is Unknown!"); 

	const UGameInstance* LocalGameInstance = GetGameInstance(); 

	// Check Forced to Show Loading Screen 
	if (LoadingScreenCVars::ForceLoadingScreenVisible)
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("LoadingScreen.AlwaysShow is true.")); 
		return true; 
	}

	// Check Game Instance has World Context Object 
	const FWorldContext* WorldContext = LocalGameInstance->GetWorldContext();
	if (WorldContext == nullptr)
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("The Game Instance Has a Null WorldContextObject.")); 
		return true; 
	}

	// Check World is not nullptr 
	UWorld* World = WorldContext->World(); 
	if (World == nullptr)
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("We have no world, FWorldContext's World() is null.")); 
		return true; 
	}

	// Check Game State Replication
	AGameStateBase* GameState = World->GetGameState<AGameStateBase>(); 
	if (GameState == nullptr)
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("GameState hasn't yet replicated, it's null.")); 
		return true; 
	}

	// Check Currently Loading Map 
	if (bCurrentlyInLoadMap)
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("bCurrentlyLoadMap is true.")); 
		return true; 
	}

	// Check Travel URL is null
	if (!WorldContext->TravelURL.IsEmpty())
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("We have pending travel, the TravelURL is empty.")); 
		return true; 
	}

	// Check Pending Net Game is null 
	if (WorldContext->PendingNetGame != nullptr)
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("We are connecting to another server, PendingNetGame != nullptr.")); 
		return true; 
	}

	// Check World Has Begun Play
	if (!World->HasBegunPlay())
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("World hasn't begun play."));
		return true; 
	}

	// Check Traveling Seamlessly 
	if (World->IsInSeamlessTravel())
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("We are in Seamless Travel.")); 
	}

	// Check if Game State needs a Loading Screen
	if (ILoadingInterface::ShouldShowLoadingScreen(GameState, DebugReasonForShowingOrHidingLoadingScreen))
	{
		return true; 
	}

	// Check Any Game State Component which needs a Loading Screen
	for (UActorComponent* GameStateComponent : GameState->GetComponents())
	{
		if (ILoadingInterface::ShouldShowLoadingScreen(GameStateComponent, DebugReasonForShowingOrHidingLoadingScreen))
		{
			return true; 
		}
	}

	// Check External Loading Processors
	for (const TWeakInterfacePtr<ILoadingInterface>& ExternalLoadingProcessor : ExternalLoadingProcessors)
	{
		if (ILoadingInterface::ShouldShowLoadingScreen(ExternalLoadingProcessor.GetObject(), DebugReasonForShowingOrHidingLoadingScreen))
		{
			return true;
		}
	}

	// Check Each Local Player
	bool bFoundAnyLocalPlayerController = false;
	bool bMissingAnyLocalPlayerController = false; 

	for (ULocalPlayer* LocalPlayer : LocalGameInstance->GetLocalPlayers())
	{
		if (LocalPlayer != nullptr)
		{
			if (APlayerController* PlayerController = LocalPlayer->PlayerController)
			{
				bFoundAnyLocalPlayerController = true; 
				
				// Check if Local Player Controller needs Loading Screen
				if (ILoadingInterface::ShouldShowLoadingScreen(PlayerController, DebugReasonForShowingOrHidingLoadingScreen))
				{
					return true; 
				}

				// Check if Local Player Controller's Components need Loading Screen
				for (UActorComponent* PlayerControllerComponent : PlayerController->GetComponents())
				{
					if (ILoadingInterface::ShouldShowLoadingScreen(PlayerControllerComponent, DebugReasonForShowingOrHidingLoadingScreen))
					{
						return true; 
					}
				}
			}
			else
			{
				bMissingAnyLocalPlayerController = true; 
			}
		}
	}

	UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient(); 
	const bool bIsInSplitscreen = GameViewportClient->GetCurrentSplitscreenConfiguration() != ESplitScreenType::None; 
	if (bIsInSplitscreen && bMissingAnyLocalPlayerController)
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("At least 1 missing local player controller in Splitscreen.")); 
	}

	if (!bIsInSplitscreen && !bFoundAnyLocalPlayerController)
	{
		DebugReasonForShowingOrHidingLoadingScreen = FString(TEXT("Need at least 1 local player controller.")); 
	}

	DebugReasonForShowingOrHidingLoadingScreen = TEXT("Nothing wans to show loading screen anymore."); 
	return false; 
}

bool ULoadingScreenManager::ShouldShowLoadingScreen()
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>(); 

	// Check for a need to show the loading screen
	const bool bNeedToShowLoadingScreen = CheckForAnyNeedToShowLoadingScreen(); 

	// Keep the loading screen up a bit longer if desired
	bool bWantToForceShowLoadingScreen = false;
	if (bNeedToShowLoadingScreen)
	{
		TimeLoadingScreenLastDismissed = -1.0; 
	}
	else
	{
		// Don't need to show the loading screen anymore, but might still want to for a bit 
		const double CurrentTime = FPlatformTime::Seconds(); 
		const bool bCanHoldLoadingScreen = (!GIsEditor || Settings->HoldAdditionalSecondsEvenInEditor); 
		const double HoldAdditionalSeconds = bCanHoldLoadingScreen ? LoadingScreenCVars::HoldAdditionalSeconds : 0.; 

		if (TimeLoadingScreenLastDismissed < 0.)
		{
			TimeLoadingScreenLastDismissed = CurrentTime; 
		}

		const double TimeSinceScreenDismissed = CurrentTime - TimeLoadingScreenLastDismissed; 
		
		// Hold for an extra seconds to cover up streaming 
		if ((HoldAdditionalSeconds > 0.) && (TimeSinceScreenDismissed < HoldAdditionalSeconds))
		{
			UGameViewportClient* GameViewportClient = GetGameInstance()->GetGameViewportClient(); 
			GameViewportClient->bDisableWorldRendering = false;

			DebugReasonForShowingOrHidingLoadingScreen = FString::Printf(
				TEXT("Keeping Loading Screen up for an additional %.2f seconds to allow Texture Streaming"),
				HoldAdditionalSeconds); 
			bWantToForceShowLoadingScreen = true; 
		}
	}

	return bNeedToShowLoadingScreen || bWantToForceShowLoadingScreen; 
}

void ULoadingScreenManager::UpdateLoadingScreen()
{
	bool bLogLoadingScreenStatus = LoadingScreenCVars::LogLoadingScreenReasonEveryFrame; 

	if (ShouldShowLoadingScreen())
	{
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>(); 

		// Make specified checkpoint in the given time to determine where progress stalled.
		FThreadHeartBeat::Get().MonitorCheckpointStart(GetFName(), Settings->HeartbeatHangDuration); 

		ShowLoadingScreen(); 

		if ((Settings->LogLoadingScreenHeartbeatInterval > 0.f) && (TimeUntilNextLogHeartbeatSeconds <= 0.0))
		{
			bLogLoadingScreenStatus = true; 
			TimeUntilNextLogHeartbeatSeconds = Settings->LogLoadingScreenHeartbeatInterval; 
		}
	}
	else
	{
		HideLoadingScreen(); 

		FThreadHeartBeat::Get().MonitorCheckpointEnd(GetFName()); 
	}
}

void ULoadingScreenManager::ShowLoadingScreen()
{
	if (bCurrentlyShowingLoadingScreen)
	{
		return;
	}

	// Unable to show loading screen if the Engine is still loading with its loading screen. 
	if (FPreLoadScreenManager::Get() && FPreLoadScreenManager::Get()->HasActivePreLoadScreenType(EPreLoadScreenTypes::EngineLoadingScreen))
	{
		return;
	}

	TimeLoadingScreenShown = FPlatformTime::Seconds(); 

	bCurrentlyShowingLoadingScreen = true; 

	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>(); 
	if (!IsShowingInitialLoadingScreen())
	{
		UGameInstance* LocalGameInstance = GetGameInstance(); 

		// Block Input 
		StartBlockingInput(); 

		// Loading Screen Visibility Changed 
		LoadingScreenVisibilityChanged.Broadcast(true); 

		// Create Loading Screen Widget
		TSubclassOf<UUserWidget> LoadingScreenWidgetClass = Settings->LoadingScreenWidget.TryLoadClass<UUserWidget>(); 
		if (UUserWidget* UserWidget = UUserWidget::CreateWidgetInstance(*LocalGameInstance, LoadingScreenWidgetClass, NAME_None))
		{
			LoadingScreenWidget = UserWidget->TakeWidget(); 
		}
		else
		{
			LoadingScreenWidget = SNew(SThrobber); 
		}

		// Add to viewport at high ZOrder to make sure loading screen widget is on top of most things
		UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient(); 
		GameViewportClient->AddViewportWidgetContent(LoadingScreenWidget.ToSharedRef(), Settings->LoadingScreenZOrder); 

		ChangePerformanceSettings(true); 

		if (!GIsEditor || Settings->ForceTickLoadingScreenEvenInEditor)
		{
			FSlateApplication::Get().Tick(); 
		}
	}
}

void ULoadingScreenManager::HideLoadingScreen()
{
	if (!bCurrentlyShowingLoadingScreen)
	{
		return; 
	}

	StopBlockingInput();

	if (!IsShowingInitialLoadingScreen())
	{
		GEngine->ForceGarbageCollection(true); 

		RemoveWidgetFromViewport(); 

		ChangePerformanceSettings(false); 

		LoadingScreenVisibilityChanged.Broadcast(false); 
	}

	const double LoadingScreenDuration = FPlatformTime::Seconds() - TimeLoadingScreenShown; 

	bCurrentlyShowingLoadingScreen = false; 
}

bool ULoadingScreenManager::IsShowingInitialLoadingScreen()
{
	FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get(); 
	return (PreLoadScreenManager != nullptr) && PreLoadScreenManager->HasValidActivePreLoadScreen();
}

void ULoadingScreenManager::StartBlockingInput()
{
	if (!InputPreProcessor.IsValid())
	{
		InputPreProcessor = MakeShareable<FLoadingScreenInputPreProcessor>(new FLoadingScreenInputPreProcessor()); 
		FSlateApplication::Get().RegisterInputPreProcessor(InputPreProcessor, 0); 
	}
}

void ULoadingScreenManager::StopBlockingInput()
{
	if (InputPreProcessor.IsValid())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputPreProcessor); 
		InputPreProcessor.Reset();
	}
}

void ULoadingScreenManager::RemoveWidgetFromViewport()
{
	UGameInstance* LocalGameInstance = GetGameInstance(); 
	if (LoadingScreenWidget.IsValid())
	{
		if (UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient())
		{
			GameViewportClient->RemoveViewportWidgetContent(LoadingScreenWidget.ToSharedRef());
		}
		LoadingScreenWidget.Reset();
	}
}

void ULoadingScreenManager::ChangePerformanceSettings(bool bEnableLoadingScreen)
{
	UGameInstance* LocalGameInstance = GetGameInstance(); 
	UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient(); 

	FShaderPipelineCache::SetBatchMode(bEnableLoadingScreen ? FShaderPipelineCache::BatchMode::Fast : FShaderPipelineCache::BatchMode::Background); 

	// Don't bother drawing 3D World while we're loading 
	GameViewportClient->bDisableWorldRendering = bEnableLoadingScreen; 

	// Make sure to prioritize streaming in levels if the loading screen is up 
	if (UWorld* ViewportWorld = GameViewportClient->GetWorld())
	{
		if (AWorldSettings* WorldSettings = ViewportWorld->GetWorldSettings(false, false))
		{
			WorldSettings->bHighPriorityLoadingLocal = bEnableLoadingScreen; 
		}
	}

	if (bEnableLoadingScreen)
	{
		// Set a new hang detector timeout multiplier when the loading screen is visible. 
		double HangDurationMultiplier; 

		if (!GConfig || !GConfig->GetDouble(TEXT("Core.System"), TEXT("LoadingScreenHangDurationMultiplier"), HangDurationMultiplier, GEngineIni))
		{
			HangDurationMultiplier = 1.0; 
		}
		FThreadHeartBeat::Get().SetDurationMultiplier(HangDurationMultiplier); 

		// Do not report hitches while the loading screen is up 
		FGameThreadHitchHeartBeat::Get().SuspendHeartBeat(); 
	}
	else
	{
		// Restore the hang detector timeout when we hide the loading screen 
		FThreadHeartBeat::Get().SetDurationMultiplier(1.0); 

		// Resume reporting hitches now that the loading screen is down 
		FGameThreadHitchHeartBeat::Get().ResumeHeartBeat(); 
	}
}



