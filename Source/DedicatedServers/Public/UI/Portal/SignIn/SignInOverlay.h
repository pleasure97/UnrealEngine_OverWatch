// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignInOverlay.generated.h"

class UJoinGame; 
class UPortalManager; 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API USignInOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UJoinGame> JoinGameWidget; 
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass; 

protected:
	virtual void NativeConstruct() override; 

private:
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager; 

	UFUNCTION()
	void OnJoinGameButtonClicked(); 
	
	UFUNCTION()
	void UpdateJoinGameStatus(const FString& StatusMessage); 
};
