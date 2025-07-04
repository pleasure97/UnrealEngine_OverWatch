// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CombatLog.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatLogExpired, UObject*, ListItemObject);

class UBorder;
class UImage;
class UTextBlock;

UENUM(BlueprintType)
enum class ECombatLogType : uint8
{
	Kill   UMETA(DisplayName = "Kill"),
	Death  UMETA(DisplayName = "Death"),
};

USTRUCT(BlueprintType)
struct FCombatLogInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> CombatLogIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor CombatLogColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CombatLogText;
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UCombatLog : public UOWUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	/* Data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Icons")
	TMap<ECombatLogType, FCombatLogInfo> CombatLogInfoMap;

	/* Widget */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_CombatLog;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_CombatLog;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Username;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AdditionalLog;

	/* Animation */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CombatLogAnimation; 

	UPROPERTY(BlueprintAssignable)
	FOnCombatLogExpired OnCombatLogExpired;

	UFUNCTION(BlueprintCallable)
	void ShowCombatLog(FHeroKilledInfo& HeroKilledInfo);

protected:
	/* User Widget */
	virtual void NativeDestruct() override; 

	/* User Object List Entry */
	virtual void NativeOnListItemObjectSet(UObject* ListItem) override;

private:
	/* Animation */
	UFUNCTION()
	void OnLogExpired();

	FTimerHandle HoldTimerHandle; 
};
