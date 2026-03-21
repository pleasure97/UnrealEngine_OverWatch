// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ListEntryMappingDataAsset.generated.h"

class UListDataObjectBase;
class UListEntryBaseWidget;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UListEntryMappingDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	TSubclassOf<UListEntryBaseWidget> FindEntryWidgetClassByDataObject(UListDataObjectBase* InDataObject) const; 

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListDataObjectBase>, TSubclassOf<UListEntryBaseWidget>> DataObjectListEntryMap;
};
