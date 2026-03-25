// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/CommonUI/CommonUIEnumTypes.h"
#include "ListEntryBaseWidget.generated.h"

class UCommonTextBlock;
class UListDataObjectBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class FPSTEMPLATE_API UListEntryBaseWidget : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(bool bWasHovered);
	
protected:
	// Child Widget Blueprint Should Override it to Handle Highlight State when This Entry Widget is Hovered or Selected
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Toggle Entry Widget Highlight State"))
	void BP_OnToggleEntryWidgetHighlightState(bool bShouldHighlight) const; 

	/* IUserObjectListEntry*/
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	// Child Class Should Override This Function to Handle Initialization Needed
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject); 

	// Child Class Should Override This Function to Update UI Values After Data Object Has been Modified. Super Call is Not Needed
	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

	// Child Class Should Override This Function to Update UI Values After Dependency Data Has been Modified. Super Call is Not Needed
	virtual void OnOwningDependencyDataObjectModified(UListDataObjectBase* OwningModifiedDependencyData, EOptionsListDataModifyReason ModifyReason);

	// Child Class Should Override This Function to Change Editable State of the Widgets it Owns. Super Call is Expected
	virtual void OnToggleEditableState(bool bIsEditable);

	void SelectThisEntryWidget();

private:
	/* Bound Widgets */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonTextBlock* CommonText_SettingDisplayName;

	UPROPERTY(Transient)
	UListDataObjectBase* CachedOwningListDataObject;
};
