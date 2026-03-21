// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/ListDataObjectBase.h"
#include "UI/CommonUI/Util/OWGameUserSettings.h"

void UListDataObjectBase::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObjectBase::OnDataObjectInitialized()
{

}

void UListDataObjectBase::NotifyListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmediately)
	{
		UOWGameUserSettings::Get()->ApplySettings(true);
	}
}