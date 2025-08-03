// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthPlate.generated.h"

class UPlayerHealthBarPool;
class UTextBlock;
class UImage;
class AOWPlayerState; 


/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealthPlate : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Widgets */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerHealthBarPool> WBP_PlayerHealthBarPool;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayerName; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_PlayerArrow; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor EnemyColor = FLinearColor(0.520833f, 0.005426f, 0.032740f, 1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor AllyColor = FLinearColor(0.f, 0.f, 1.f, 1.f);

	UFUNCTION(BlueprintCallable)
	void SetPlayerState(AOWPlayerState* InOWPlayerState);

	AOWPlayerState* GetOWPlayerState() const; 

protected:
	virtual void NativeDestruct() override; 

private:
	UPROPERTY()
	TObjectPtr<AOWPlayerState> OWPlayerState;

	UFUNCTION()
	void OnTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID); 

	void UpdatePlayerInfo(bool bAlly);
	void UpdatePlayerName(bool bAlly);
	void UpdatePlayerArrow(bool bAlly); 
	void UpdatePlayerHealthBarPool(bool bAlly);

	int32 OwnerTeamID = -1; 
};
