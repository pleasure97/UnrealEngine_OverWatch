// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerOverlay.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/HealthBarPool.h"
#include "UI/Widget/PlayerHealthStatus.h"
#include "UI/Widget/PlayerSkills.h"
#include "UI/Widget/PlayerWeaponStatus.h"
#include "UI/Widget/UltimateGauge.h"
#include "UI/Widget/HitIndicatorPool.h"
#include "UI/Widget/CombatLogPool.h"
#include "UI/Widget/KillLogPool.h"

void UPlayerOverlay::SetChildWidgetControllers()
{
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		WBP_PlayerHealthBarPool->SetWidgetController(OverlayWidgetController); 
		WBP_PlayerHealthStatus->SetWidgetController(OverlayWidgetController);
		WBP_PlayerSkills->SetWidgetController(OverlayWidgetController); 
		WBP_PlayerWeaponStatus->SetWidgetController(OverlayWidgetController); 
		WBP_UltimateGauge->SetWidgetController(OverlayWidgetController); 
		WBP_HitIndicatorPool->SetWidgetController(OverlayWidgetController); 
		WBP_CombatLogPool->SetWidgetController(OverlayWidgetController); 
		WBP_KillLogPool->SetWidgetController(OverlayWidgetController); 
	}
}
