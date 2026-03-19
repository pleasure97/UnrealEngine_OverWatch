// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/ListDataObjectCollection.h"

void UListDataObjectCollection::AddChildListData(UListDataObjectBase* InChildListData)
{
	// Notify Child List Data Array to Initialize itself
	InChildListData->InitDataObject();

	// Set Child List Data's Parent to this 
	InChildListData->SetParentData(this);

	ChildListDataArray.Add(InChildListData);
}

TArray<UListDataObjectBase*> UListDataObjectCollection::GetChildListData() const
{
	return ChildListDataArray;
}

bool UListDataObjectCollection::HasChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}


