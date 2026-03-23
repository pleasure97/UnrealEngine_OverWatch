// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UI/CommonUI/CommonUIEnumTypes.h"
#include "UI/CommonUI/CommonUIStructTypes.h"
#include "ListDataObjectBase.generated.h"

#define LIST_DATA_ACCESORS(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; }	\
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }
/**
 * 
 */
UCLASS(Abstract)
class FPSTEMPLATE_API UListDataObjectBase : public UObject
{
	GENERATED_BODY()
	
public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObjectBase*, EOptionsListDataModifyReason);

	FOnListDataModifiedDelegate OnListDataModified;
	FOnListDataModifiedDelegate OnDependencyDataModified;

	LIST_DATA_ACCESORS(FName, DataID);
	LIST_DATA_ACCESORS(FText, DataDisplayName);
	LIST_DATA_ACCESORS(FText, DataDescriptionText);
	LIST_DATA_ACCESORS(FText, DisabledText);
	LIST_DATA_ACCESORS(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage);
	LIST_DATA_ACCESORS(UListDataObjectBase*, ParentData);

	void InitDataObject();

	// Empty in Base Class. Child Class ListDataObjectCollection Should Override it
	// The Function Should Return All Children Data Tab Has
	virtual TArray<UListDataObjectBase*> GetAllChildListData() const { return TArray<UListDataObjectBase*>(); }
	virtual bool HasAnyChildListData() const { return false; }

	void SetShouldApplySettingsImmediately(bool bInShouldApplyChangeImmediately) { bShouldApplyChangeImmediately = bInShouldApplyChangeImmediately; }

	/* Reset Default Value */
	// Child Class Should Override Functions to Provide Implementations for Resetting Data 
	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }

	// Gets Called from OptionDataRegistry for Adding in Edit Conditions for Constructed List Data Objects
	void AddEditCondition(const FOptionDataEditConditionDescriptor& InEditCondition);

	void AddEditDependencyData(UListDataObjectBase* InDependencyData);

	bool IsDataCurrentlyEditable();

protected:
	// Child Classes Should Override it to Handle Initialized Needed Accordingly
	virtual void OnDataObjectInitialized(); 

	virtual void NotifyListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);

	// Child Classes Should Override it to Allow the Value to be Set to the Forced String Value
	virtual bool CanSetToForcedStringValue(const FString& InForcedStringValue) const { return false; }

	// Child Classes Should Override it to Specify How to Set Current Value to Forced Value
	virtual void OnSetToForcedStringValue(const FString& InForcedStringValue) {}

	virtual void OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason);

private:
	FName DataID;
	FText DataDisplayName;
	FText DataDescriptionText;
	FText DisabledText;

	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UListDataObjectBase* ParentData;

	bool bShouldApplyChangeImmediately = false;

	UPROPERTY(Transient)
	TArray<FOptionDataEditConditionDescriptor> EditConditionDescriptorArray;
};
