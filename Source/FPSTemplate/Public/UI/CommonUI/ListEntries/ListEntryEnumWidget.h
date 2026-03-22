// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/ListEntries/ListEntryBaseWidget.h"
#include "ListEntryEnumWidget.generated.h"

class UComboBoxString;
class UListDataObjectStringEnum;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class FPSTEMPLATE_API UListEntryEnumWidget : public UListEntryBaseWidget
{
	GENERATED_BODY()
	
protected:
	/* UWidget */
	virtual void NativeOnInitialized() override;

	/* UListEntryBaseWidget */
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject) override;

	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;

private:
	/* Callbacks */
	UFUNCTION()
	void HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	/* Bound Widgets */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UComboBoxString* ComboBoxString_Options;
	/* Bound Widgets End */

	UPROPERTY(Transient)
	UListDataObjectStringEnum* CachedOwningStringEnumDataObject;
};
