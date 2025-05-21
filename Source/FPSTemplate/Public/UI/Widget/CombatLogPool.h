// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "UI/Widget/CombatLog.h"
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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> ListView_CombatLog; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ListView")
	TSubclassOf<UCombatLog> ListItemClass; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ListView")
	int32 MaxItems = 5; 

	UFUNCTION()
	void OnHeroKilled(FGameplayTag Channel, const FHeroKilledInfo& Payload);

	UFUNCTION(BlueprintCallable)
	void AddNewItem(ECombatLogType CombatLogType, const FString& PlayerName);
	
protected:
	virtual void NativeConstruct() override; 
};
