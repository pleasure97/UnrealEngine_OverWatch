// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "CombatLog.generated.h"

DECLARE_DELEGATE_OneParam(FOnCombatLogFinished, UCombatLog*);

class UBorder;
class UImage;
class UTextBlock;

namespace CombatLogColors
{
	constexpr FLinearColor Gray(0.520833f, 0.520833f, 0.520833f, 1.f);
	constexpr FLinearColor Red(0.520833f, 0.005426f, 0.032740f, 1.f);
}; 

UENUM(BlueprintType)
enum class ECombatLogType : uint8
{
	Kill   UMETA(DisplayName = "Kill"),
	Death  UMETA(DisplayName = "Death"),
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UCombatLog : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	/* Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_CombatLog;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_CombatLog;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Username;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_KillLog;

	/* Animation */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* StartAnimation; 

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* EndAnimation; 

	FOnCombatLogFinished OnCombatLogFinished; 

	void HoldThenPlayEnd();

	/* Icon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Icons")
	TMap<ECombatLogType, UTexture2D*> IconMap;

	UFUNCTION(BlueprintCallable)
	void ShowCombatLog(ECombatLogType CombatLogType, const FString& PlayerName); 

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
