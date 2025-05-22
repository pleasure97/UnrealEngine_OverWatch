// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> ListView_KillLog; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ListView")
	TSubclassOf<UKillLog> ListItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ListView")
	int32 MaxItems = 5;

	UFUNCTION()
	void OnHeroKilled(FGameplayTag Channel, const FHeroKilledInfo& Payload);

	UFUNCTION(BlueprintCallable)
	void AddNewItem(EKillLogType KillLogType, const AOWPlayerState* InstigatorPlayerState, const AOWPlayerState* VictimPlayerState);

protected:
	virtual void NativeConstruct() override; 
};
