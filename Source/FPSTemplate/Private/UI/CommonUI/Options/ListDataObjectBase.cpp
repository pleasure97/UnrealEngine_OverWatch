// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/ListDataObjectBase.h"
#include "UI/CommonUI/Util/OWGameUserSettings.h"

void UListDataObjectBase::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObjectBase::AddEditCondition(const FOptionDataEditConditionDescriptor& InEditCondition)
{
	EditConditionDescriptorArray.Add(InEditCondition);
}

void UListDataObjectBase::AddEditDependencyData(UListDataObjectBase* InDependencyData)
{
	if (!InDependencyData->OnListDataModified.IsBoundToObject(this))
	{
		InDependencyData->OnListDataModified.AddUObject(this, &UListDataObjectBase::OnEditDependencyDataModified);
	}
}

bool UListDataObjectBase::IsDataCurrentlyEditable()
{
	bool bIsEditable = true; 

	if (EditConditionDescriptorArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;

	for (const FOptionDataEditConditionDescriptor& EditConditionDescriptor : EditConditionDescriptorArray)
	{
		if (!EditConditionDescriptor.IsValid() || EditConditionDescriptor.IsEditConditionMet())
		{
			continue;
		}

		bIsEditable = false;

		CachedDisabledRichReason.Append(EditConditionDescriptor.GetDisabledRichReason());

		SetDisabledText(FText::FromString(CachedDisabledRichReason));

		if (EditConditionDescriptor.HasForcedStringValue())
		{
			const FString ForcedStringValue =  EditConditionDescriptor.GetDisabledForcedStringValue();

			// If the current value this object data has can be set to forced value
			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}

	return bIsEditable;
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

void UListDataObjectBase::OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	OnDependencyDataModified.Broadcast(ModifiedDependencyData, ModifyReason);
}
