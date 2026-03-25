// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/ListEntries/ListEntryBaseWidget.h"
#include "ListEntryKeyRemapWidget.generated.h"

class UOWCommonButtonBase;
class UListDataObjectKeyRemap;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class FPSTEMPLATE_API UListEntryKeyRemapWidget : public UListEntryBaseWidget
{
	GENERATED_BODY()
	
protected:
	/* UUserWidget */
	virtual void NativeOnInitialized() override;

	/* UListEntryBaseWidget */
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject) override;

	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;

private:
	void OnRemapKeyButtonClicked();

	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyRemapCanceled(const FString& CanceldReason);

	void UpdateMappingKeyDisplay(); 

	/* Bound Widgets */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess="true"))
	UOWCommonButtonBase* CommonButton_KeyboardRemapKey;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UOWCommonButtonBase* CommonButton_SecondaryRemapKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UOWCommonButtonBase* CommonButton_GamepadRemapKey;

	UPROPERTY(Transient)
	UListDataObjectKeyRemap* CachedOwningKeyRemapDataObject;
};
