// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/ActivatableBaseWidget.h"
#include "OptionScreenWidget.generated.h"

class UOptionDataRegistry;
class UOWCommonTabListWidgetBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class FPSTEMPLATE_API UOptionScreenWidget : public UActivatableBaseWidget
{
	GENERATED_BODY()
	
public:
	void RequestRegisterTab(const FName&); 

protected:
	/* User Widget */
	virtual void NativeOnInitialized() override;
	/* User Widget Ends */

	/* UCommonActivatableWidget */
	virtual void NativeOnActivated() override;
	/* UCommonActivatableWidget Ends */

private:
	UOptionDataRegistry* GetOrCreateDataRegistry(); 

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	/* Bound Widgets */
	UPROPERTY(meta = (BindWidget))
	UOWCommonTabListWidgetBase* TabListWidget_OptionsTab;
	/* Bound Widgets End */

	// Handle Creation of Data in Option Screen. Direct Access to This Variable is Forbidden
	UPROPERTY(Transient)
	UOptionDataRegistry* OwningDataRegistry;

	UPROPERTY(EditDefaultsOnly, Category = "Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;
};
