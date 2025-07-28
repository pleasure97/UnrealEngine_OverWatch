// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DSPlayerController.h"
#include "Interface/TeamInterface.h"
#include "GameplayTagContainer.h"
#include "OWPlayerController.generated.h"

class UInputMappingContext;
class UInputAction; 
class UOWInputConfig; 
class UOWAbilitySystemComponent; 
struct FInputActionValue; 
class AOWPlayerState; 
class UUserWidget; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWPlayerController : public ADSPlayerController, public ITeamInterface 
{
	GENERATED_BODY()
	
public:
	AOWPlayerController(); 

	/** Team Interface **/
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override; 
	virtual FGenericTeamId GetGenericTeamId() const override; 
	virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override; 
	/** Team Interface End **/

	UFUNCTION(Server, Reliable)
	void ServerChooseHero(EHeroName ChosenHero); 

	UFUNCTION(BlueprintCallable)
	void ShowWidget(TSubclassOf<UUserWidget> InUserWidget); 

	UFUNCTION(BlueprintCallable)
	void CollapseWidget(TSubclassOf<UUserWidget> InUserWidget); 

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override; 
	virtual void SetupInputComponent() override; 

	/** AController Interface **/
	virtual void InitPlayerState() override; 
	virtual void CleanupPlayerState() override; 
	virtual void OnRep_PlayerState() override; 
	/** AController Interface End **/

	bool bPlayerAlive = false; 

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> OWContext; 

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction; 

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UOWInputConfig> InputConfig; 

	UPROPERTY()
	TObjectPtr<UOWAbilitySystemComponent> OWAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState; 

	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate; 

	UOWAbilitySystemComponent* GetAbilitySystemComponent();

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void AbilityInputTagPressed(FGameplayTag InputTag); 
	void AbilityInputTagReleased(FGameplayTag InputTag); 
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void BroadcastOnPlayerStateChanged(); 

	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam); 
};
