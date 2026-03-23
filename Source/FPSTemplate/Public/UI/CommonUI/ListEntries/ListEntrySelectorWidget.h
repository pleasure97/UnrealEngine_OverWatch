// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/ListEntries/ListEntryBaseWidget.h"
#include "ListEntrySelectorWidget.generated.h"

class UOWCommonButtonBase;
class UOWCommonRotator;
class UListDataObjectString;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class FPSTEMPLATE_API UListEntrySelectorWidget : public UListEntryBaseWidget
{
	GENERATED_BODY()

protected:
	/* UWidget */
	virtual void NativeOnInitialized() override;

	/* UListEntryBaseWidget */
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject) override;

	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;

	virtual void OnToggleEditableState(bool bIsEditable) override;

private:
	void OnPreviousOptionClicked(); 
	void OnNextOptionClicked(); 

	/* Bound Widgets */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UOWCommonButtonBase* CommonButton_PreviousOption;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UOWCommonRotator* CommonRotator_AvailableOptions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UOWCommonButtonBase* CommonButton_NextOption;

	UPROPERTY(Transient)
	UListDataObjectString* CachedOwningStringDataObject; 
};
