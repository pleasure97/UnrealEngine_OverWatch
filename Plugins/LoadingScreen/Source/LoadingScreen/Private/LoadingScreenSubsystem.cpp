// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenSubsystem.h"

ULoadingScreenSubsystem::ULoadingScreenSubsystem()
{
}

void ULoadingScreenSubsystem::SetLoadingScreenWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (LoadingScreenWidgetClass != NewWidgetClass)
    {
        LoadingScreenWidgetClass = NewWidgetClass; 
        OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass); 
    }
}

TSubclassOf<UUserWidget> ULoadingScreenSubsystem::GetLoadingScreenWidget() const
{
    return LoadingScreenWidgetClass; 
}
