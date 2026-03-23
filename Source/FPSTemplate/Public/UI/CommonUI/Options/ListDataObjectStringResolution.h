// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/Options/ListDataObjectString.h"
#include "ListDataObjectStringResolution.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UListDataObjectStringResolution : public UListDataObjectStringEnum
{
	GENERATED_BODY()
	
public:
	void InitResolutionValues();

	FString GetMaximumAllowedResolution() const { return MaximumAllowedResolution; }

protected:
	/* UListDataObjectString */
	virtual void OnDataObjectInitialized() override;

private:
	FString ResolutionToValueString(const FIntPoint& InResolution) const;
	FText ResolutionToDisplayText(const FIntPoint& InResolution) const;

	FString MaximumAllowedResolution;
};
