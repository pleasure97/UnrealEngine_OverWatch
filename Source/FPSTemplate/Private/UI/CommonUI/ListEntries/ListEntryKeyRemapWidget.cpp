// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/ListEntries/ListEntryKeyRemapWidget.h"
#include "UI/CommonUI/Options/ListDataObjectKeyRemap.h"
#include "UI/CommonUI/Components/OWCommonButtonBase.h"
#include "UI/CommonUI/Util/CommonUISubsystem.h"
#include "OWGameplayTags.h"
#include "UI/CommonUI/Util/CommonUIFunctionLibrary.h"
#include "UI/CommonUI/Options/KeyRemapScreenWidget.h"

void UListEntryKeyRemapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized(); 

	CommonButton_KeyboardRemapKey->OnClicked().AddUObject(this, &UListEntryKeyRemapWidget::OnRemapKeyButtonClicked);
}

void UListEntryKeyRemapWidget::OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObjectKeyRemap>(InOwningListDataObject);

	// TODO - Binding Second Key
	UpdateMappingKeyDisplay();
}

void UListEntryKeyRemapWidget::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		// TODO - Binding Second Key
		UpdateMappingKeyDisplay();
	}
}

void UListEntryKeyRemapWidget::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();

	UCommonUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		CommonUIGameplayTags::CommonUI_WidgetStack_Modal,
		UCommonUIFunctionLibrary::GetCommonUISoftWidgetClassByTag(CommonUIGameplayTags::CommonUI_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, UActivatableBaseWidget* PushedWidget)
		{
			switch (PushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
			{
				UKeyRemapScreenWidget* CreatedKeyRemapScreen = CastChecked<UKeyRemapScreenWidget>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &UListEntryKeyRemapWidget::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &UListEntryKeyRemapWidget::OnKeyRemapCanceled);
				if (CachedOwningKeyRemapDataObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputType());
				}
				break;
			}
			case EAsyncPushWidgetState::AfterPush:
			{
				break;
			}
			default:
				break;
			}
		}
	);
}

void UListEntryKeyRemapWidget::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UListEntryKeyRemapWidget::OnKeyRemapCanceled(const FString& CanceldReason)
{
	// TODO - If Confirm Screen or Other Widget to Notify is Needed
	// UCommonUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync();
}

void UListEntryKeyRemapWidget::UpdateMappingKeyDisplay()
{
	FSlateBrush IconSlateBrush = CachedOwningKeyRemapDataObject->GetIconFromCurrentKey();

	if (IconSlateBrush.IsSet() && IconSlateBrush.GetResourceObject() != nullptr)
	{
		CommonButton_KeyboardRemapKey->SetButtonDisplayImage(IconSlateBrush);
		CommonButton_KeyboardRemapKey->SetButtonDisplayMode(true /* bShowImage*/);
	}
	else
	{
		CommonButton_KeyboardRemapKey->SetButtonDisplayText(CachedOwningKeyRemapDataObject->GetDisplayTextFromCurrentKey());
		CommonButton_KeyboardRemapKey->SetButtonDisplayMode(false /* bShowImage*/);
	}
}
