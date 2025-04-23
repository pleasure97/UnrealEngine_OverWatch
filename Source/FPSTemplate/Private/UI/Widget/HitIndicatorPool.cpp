// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HitIndicatorPool.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/HitIndicator.h"
#include "Components/Overlay.h"
#include "AbilitySystem/OWAttributeSet.h"

void UHitIndicatorPool::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (!HitIndicatorClass) { return; }

	check(WidgetController); 

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController); 

		if (UOWAttributeSet* OWAttributeSet = Cast<UOWAttributeSet>(OverlayWidgetController->AttributeSet))
		{
			OWAttributeSet->OnDamageReceived.AddUObject(this, &UHitIndicatorPool::ProcessDamageReceived); 
		}
	}

	for (int32 i = 0; i < NumHitIndicators; ++i)
	{
		UHitIndicator* HitIndicator = CreateWidget<UHitIndicator>(this, HitIndicatorClass);
		Overlay_HitIndicatorPool->AddChild(HitIndicator); 
		HitIndicator->SetVisibility(ESlateVisibility::Collapsed); 
		HitIndicator->OnHitIndicatorEnd.AddUObject(this, &UHitIndicatorPool::OnHitIndicatorEnd); 
		IndicatorQueue.Add(HitIndicator); 
	}
}

void UHitIndicatorPool::ProcessDamageReceived(AActor* DamageCauser, AActor* OwnerActor, float Damage)
{
	if (!DamageCauser || !OwnerActor) { return; }

	UHitIndicator* ChosenHitIndicator = nullptr; 
	
	// Find the Hit Indicator which is in Idle State in Pool 
	for (int32 i = 0; i < IndicatorQueue.Num(); ++i)
	{
		if (IndicatorQueue[i]->GetHitIndicatorState() == EHitIndicatorState::Idle)
		{
			ChosenHitIndicator = IndicatorQueue[i]; 
			IndicatorQueue.RemoveAt(i); 
			break; 
		}
	}

	// If there is No Idle Hit Indicator in the Pool, Select Hit Indicators in Order and Execute End Animation 
	if (!ChosenHitIndicator)
	{
		for (int32 i = 0; i < IndicatorQueue.Num(); ++i)
		{
			if (IndicatorQueue[i]->GetHitIndicatorState() != EHitIndicatorState::Ending)
			{
				ChosenHitIndicator = IndicatorQueue[i];
				IndicatorQueue.RemoveAt(i);
				break;
			}
		}
	}

	// TODO - 
	if (!ChosenHitIndicator) { return; }

	IndicatorQueue.Add(ChosenHitIndicator); 

	ChosenHitIndicator->PlayStart(DamageCauser, OwnerActor, Damage); 
}

void UHitIndicatorPool::OnHitIndicatorEnd(UHitIndicator* HitIndicator)
{
	int32 QueueIndex = IndicatorQueue.IndexOfByKey(HitIndicator); 
	if (QueueIndex != INDEX_NONE)
	{
		IndicatorQueue.RemoveAt(QueueIndex);
		IndicatorQueue.Add(HitIndicator); 
	}
}

