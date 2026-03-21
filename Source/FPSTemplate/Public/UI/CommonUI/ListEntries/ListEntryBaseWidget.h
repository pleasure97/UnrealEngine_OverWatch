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
	/* IUserObjectListEntry*/
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	// Child Class Should Override This Function to Handle Initialization Needed
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject); 

	// Child Class Should Override This Function to Update UI Values After Data Object Has been Modified. Super Call is Not Needed
	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

	void SelectThisEntryWidget();

private:
	/* Bound Widgets */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
};
