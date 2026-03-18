// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/ActivatableBaseWidget.h"
#include "player/OWPlayerController.h"

AOWPlayerController* UActivatableBaseWidget::GetOwningOWPlayerController()
{
    if (!CachedOwningPlayerController.IsValid())
    {
        CachedOwningPlayerController = GetOwningPlayer<AOWPlayerController>();
    }
    return CachedOwningPlayerController.IsValid() ? CachedOwningPlayerController.Get() : nullptr;
}
