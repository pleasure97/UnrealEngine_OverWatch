// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "IndicatorLayer.generated.h"

class SActorCanvas; 
class SWidget; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UIndicatorLayer : public UWidget
{
	GENERATED_BODY()
	
public:
	UIndicatorLayer(); 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Appearance")
	FSlateBrush ArrowBrush; 

protected:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override; 
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
	TSharedPtr<SActorCanvas> MyActorCanvas; 
};
