// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardOverlay.generated.h"

class UWidgetSwitcher; 
class UOverlay; 
class UGamePage; 
class UCareerPage; 
class ULeaderboardPage; 
class UEndGamePage; 
class UButton;
class UGameStatsManager; 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDashboardOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Widget Switcher */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher; 

	/* Main Page */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_MainPage;

	/* Game Page */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGamePage> GamePage; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Game;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> GameButtonHovered; 

	/* Career Page */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCareerPage> CareerPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Career;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CareerButtonHovered;

	/* Leaderboard Page */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULeaderboardPage> LeaderboardPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Leaderboard;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LeaderboardButtonHovered;
	
	/* Setting Page */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Setting; 

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SettingButtonHovered;

	/* End Game Page */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEndGamePage> EndGamePage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_EndGame;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> EndGameButtonHovered;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass; 

protected:
	virtual void NativeConstruct() override; 

	// Make focusable so that overlay can receive key inputs 
	virtual bool Initialize() override; 

	// Override Key Down Event
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override; 
	
private:
	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager; 

	/* Main Page */
	UFUNCTION()
	void ShowMainPage(); 

	/* Game Page */
	UFUNCTION()
	void ShowGamePage(); 

	UFUNCTION()
	void OnButtonGameHovered();

	UFUNCTION()
	void OnButtonGameUnhovered();

	/* Career Page */
	UFUNCTION()
	void ShowCareerPage();

	UFUNCTION()
	void OnButtonCareerHovered();

	UFUNCTION()
	void OnButtonCareerUnhovered();

	/* Leaderboard Page */
	UFUNCTION()
	void ShowLeaderboardPage(); 

	UFUNCTION()
	void OnButtonLeaderboardHovered();

	UFUNCTION()
	void OnButtonLeaderboardUnhovered();

	/* Setting Page */
	UFUNCTION()
	void ShowSettingPage(); 

	UFUNCTION()
	void OnButtonSettingHovered();

	UFUNCTION()
	void OnButtonSettingUnhovered();

	/* EndGame Page */
	UFUNCTION()
	void ShowEndGamePage(); 

	UFUNCTION()
	void OnButtonEndGameHovered();

	UFUNCTION()
	void OnButtonEndGameUnhovered();
};
