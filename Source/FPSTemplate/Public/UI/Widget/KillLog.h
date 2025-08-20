// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "KillLog.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillLogExpired, UObject*, ListItemObject);

class UBorder; 
class UTextBlock; 
class UImage; 
class AOWPlayerState; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UKillLog : public UOWUserWidget, public IUserObjectListEntry
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

	UPROPERTY(BlueprintAssignable)
	FOnKillLogExpired OnKillLogExpired;

	/* Animation */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* KillLogAnimation;

	UFUNCTION(BlueprintCallable)
	void ShowKillLog(FHeroKilledInfo& HeroKilledInfo);

	/* Color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor BlueColor = FLinearColor(0.041667f, 0.6407f, 1.f, 1.f);;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor RedColor = FLinearColor(0.520833f, 0.005426f, 0.032740f, 1.f);

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
