// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "KillLogPool.generated.h"

class UListView; 
class UKillLog; 
struct FHeroKilledInfo; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UKillLogPool : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	/* List View */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UListView> ListView_KillLog; 

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	UFUNCTION(BlueprintCallable)
	void UpdateDisplayVisibility(); 

	UFUNCTION()
	void OnHeroKilled(FGameplayTag Channel, const FHeroKilledInfo& Payload);

	void ClearKillLogPool(); 

	FGameplayMessageListenerHandle HeroKilledListener; 
};
