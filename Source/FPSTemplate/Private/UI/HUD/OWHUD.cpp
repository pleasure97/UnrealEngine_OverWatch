// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/OWHUD.h"
#include "UI/Widget/OWUserWidget.h"

void AOWHUD::BeginPlay()
{
	Super::BeginPlay(); 

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport(); 
}
