// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/ActivatableBaseWidget.h"
#include "UI/CommonUI/CommonUIEnumTypes.h"
#include "OptionScreenWidget.generated.h"

class UOptionDataRegistry;
class UOWCommonTabListWidgetBase;
class UOWCommonListView;
class UListDataObjectBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class FPSTEMPLATE_API UOptionScreenWidget : public UActivatableBaseWidget
{
	GENERATED_BODY()
	
protected:
	/* User Widget */
	virtual void NativeOnInitialized() override;
	/* User Widget Ends */

	/* UCommonActivatableWidget */
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	// Override to provide the desired widget that should receive focus when this becomes the primary active widget.
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	/* UCommonActivatableWidget Ends */

private:
	UOptionDataRegistry* GetOrCreateDataRegistry(); 

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionTabSelected(FName TabId);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	void OnListViewListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason);

	/* Bound Widgets */
	UPROPERTY(meta = (BindWidget))
	UOWCommonTabListWidgetBase* TabListWidget_OptionsTab;

	UPROPERTY(meta = (BindWidget))
	UOWCommonListView* CommonListView_OptionsList;
	/* Bound Widgets End */

	// Handle Creation of Data in Option Screen. Direct Access to This Variable is Forbidden
	UPROPERTY(Transient)
	UOptionDataRegistry* OwningDataRegistry;

	UPROPERTY(EditDefaultsOnly, Category = "Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

	UPROPERTY(Transient)
	TArray<UListDataObjectBase*> ResettableDataArray;

	bool bIsResettingData = false;
};
