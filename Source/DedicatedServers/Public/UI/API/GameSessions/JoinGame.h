// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoinGame.generated.h"

class UButton; 
class UTextBlock; 
class USizeBox; 
class UImage; 
class UBorder; 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UJoinGame : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// TODO - Timer Widget 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_JoinGame; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_FindingGame; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_LoadingThrobber; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_BeforeFindingGame; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_AfterFindingGame; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MainDescription; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SubDescription;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_CancelJoinGame; 

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MID_LoadingThrobber; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString FindActiveGameSessionString; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ConnectingToServerString;

	UFUNCTION()
	void ProcessHTTPStatus(const FString& Message, bool bShouldResetWidgets); 

protected:
	virtual void NativeConstruct() override; 

private:
	UFUNCTION()
	void OnJoiningGameCanceled();
	void RollbackOriginalWidget();

};
