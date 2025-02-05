// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerSkill.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerSkill::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	ProgressBar_Cooltime->SetPercent(0.f); 
	TextBlock_Cooltime->SetRenderOpacity(0.f); 
}
