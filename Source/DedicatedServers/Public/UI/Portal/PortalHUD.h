// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Portal/Interfaces/HUDManagement.h"
#include "PortalHUD.generated.h"

class USignInOverlay;
class UDashboardOverlay; 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API APortalHUD : public AHUD, public IHUDManagement
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USignInOverlay> SignInOverlayClass; 

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDashboardOverlay> DashboardOverlayClass; 

	/* HUDManagement Interface */
	virtual void OnSignIn() override; 
	virtual void OnSignOut() override; 
	/* HUDManagement Interface ends */


protected:
	virtual void BeginPlay() override; 

private:
	UPROPERTY()
	TObjectPtr<USignInOverlay> SignInOverlay; 
	
	UPROPERTY()
	TObjectPtr<UDashboardOverlay> DashboardOverlay; 
};
