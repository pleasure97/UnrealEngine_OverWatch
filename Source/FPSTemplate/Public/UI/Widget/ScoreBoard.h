// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "ScoreBoard.generated.h"

class UScoreBoardItem; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UScoreBoard : public UOWUserWidget
{
	GENERATED_BODY()

public:
	/* Ally */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_AllyPlayer1; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_AllyPlayer2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_AllyPlayer3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_AllyPlayer4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_AllyPlayer5;
	
	/* Enemy */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_EnemyPlayer1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_EnemyPlayer2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_EnemyPlayer3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_EnemyPlayer4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreBoardItem> ScoreBoardItem_EnemyPlayer5;

protected:
	virtual void NativeConstruct() override; 
};
