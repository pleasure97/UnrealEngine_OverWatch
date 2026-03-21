// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/ListEntryMappingDataAsset.h"
#include "UI/CommonUI/Options/ListDataObjectBase.h"

TSubclassOf<UListEntryBaseWidget> UListEntryMappingDataAsset::FindEntryWidgetClassByDataObject(UListDataObjectBase* InDataObject) const
{
    check(InDataObject); 

    for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
    {
        if (TSubclassOf<UListDataObjectBase> ConvertedDataObjectClass = TSubclassOf<UListDataObjectBase>(DataObjectClass))
        {
            if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
            {
                return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
            }
        }
    }

    return TSubclassOf<UListEntryBaseWidget>();
}
