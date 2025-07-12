// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "UI/Widget/CombatLog.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "CombatLogPool.generated.h"

class UListView; 
struct FHeroKilledInfo; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UCombatLogPool : public UOWUserWidget
{
	GENERATED_BODY()

public:
	/* List View */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UListView> ListView_CombatLog;
	
protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 
	
private:
	UFUNCTION(BlueprintCallable)
	void UpdateDisplayVisibility(); 

	UFUNCTION()
	void OnHeroKilled(FGameplayTag Channel, const FHeroKilledInfo& Payload);

	void ClearCombatLogPool(); 

	FGameplayMessageListenerHandle HeroKilledListener; 
};
