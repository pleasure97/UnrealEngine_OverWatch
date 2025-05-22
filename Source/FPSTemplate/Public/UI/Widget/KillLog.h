// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "KillLog.generated.h"

DECLARE_DELEGATE_OneParam(FOnKillLogFinished, UKillLog*);

class UBorder; 
class UTextBlock; 
class UImage; 
class AOWPlayerState; 

namespace KillLogColors
{
	constexpr FLinearColor Blue(0.041667f, 0.6407f, 1.f, 1.f);
	constexpr FLinearColor Red(0.520833f, 0.005426f, 0.032740f, 1.f);
};

UENUM(BlueprintType)
enum class EKillLogType : uint8
{
	AllyKill   UMETA(DisplayName = "AllyKill"),
	EnemyKill  UMETA(DisplayName = "EnemyKill"),
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UKillLog : public UOWUserWidget
{
	GENERATED_BODY()

public:
	/* Instigator */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Kill; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Kill; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Portrait_Kill; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_KillSkill; 

	/* Victim */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Dead;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Dead;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Portrait_Dead;

	FOnKillLogFinished OnKillLogFinished;

	void HoldThenPlayEnd();

	/* Animation */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* StartAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* EndAnimation;

	UFUNCTION(BlueprintCallable)
	void ShowKillLog(EKillLogType KillLogType, const AOWPlayerState* InstigatorPlayerState, const AOWPlayerState* VictimPlayerState);

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

	/* Animation */
	UFUNCTION()
	void OnStartAnimationFinished();

	UFUNCTION()
	void OnEndAnimationFinished();

	FTimerHandle HoldTimerHandle;
};
