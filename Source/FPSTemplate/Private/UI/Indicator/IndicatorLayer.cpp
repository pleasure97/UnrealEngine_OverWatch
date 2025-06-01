// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Indicator/IndicatorLayer.h"
#include "UI/Indicator/SActorCanvas.h"
#include "Widgets/Layout/SBox.h"

UIndicatorLayer::UIndicatorLayer()
{
	bIsVariable = true;
	SetVisibility(ESlateVisibility::HitTestInvisible); 
}

void UIndicatorLayer::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren); 

	MyActorCanvas.Reset(); 
}

TSharedRef<SWidget> UIndicatorLayer::RebuildWidget()
{
	if (!IsDesignTime())
	{
		ULocalPlayer* LocalPlayer = GetOwningLocalPlayer(); 
		if (ensureMsgf(LocalPlayer, TEXT("Attempting to rebuild a UActorCanvas without a valid LocalPlayer!")))
		{
			MyActorCanvas = SNew(SActorCanvas, FLocalPlayerContext(LocalPlayer), &ArrowBrush);
			return MyActorCanvas.ToSharedRef();
		}
	}

	return SNew(SBox); 
}


