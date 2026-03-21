// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/ConfirmScreenWidget.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "UI/CommonUI/Components/OWCommonButtonBase.h"
#include "ICommonInputModule.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage, const FText& Option1Text)
{
    UConfirmScreenInfoObject* ConfirmScreenInfoObject = NewObject<UConfirmScreenInfoObject>();

    ConfirmScreenInfoObject->ScreenTitle = InScreenTitle;
    ConfirmScreenInfoObject->ScreenMessage = InScreenMessage;

    FConfirmScreenButtonInfo OKButtonInfo;
    OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
    OKButtonInfo.ButtonTextToDisplay = Option1Text;

    ConfirmScreenInfoObject->AvailableScreenButtons.Add(OKButtonInfo); 

    return ConfirmScreenInfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage, const FText& Option1Text, const FText& Option2Text)
{
    UConfirmScreenInfoObject* ConfirmScreenInfoObject = NewObject<UConfirmScreenInfoObject>();

    ConfirmScreenInfoObject->ScreenTitle = InScreenTitle;
    ConfirmScreenInfoObject->ScreenMessage = InScreenMessage;

    FConfirmScreenButtonInfo YesButtonInfo;
    YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    YesButtonInfo.ButtonTextToDisplay = Option1Text;

    FConfirmScreenButtonInfo NoButtonInfo;
    NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
    NoButtonInfo.ButtonTextToDisplay = Option2Text;

    ConfirmScreenInfoObject->AvailableScreenButtons.Add(YesButtonInfo);
    ConfirmScreenInfoObject->AvailableScreenButtons.Add(NoButtonInfo);

    return ConfirmScreenInfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateNoYesScreen(const FText& InScreenTitle, const FText& InScreenMessage, const FText& Option1Text, const FText& Option2Text)
{
    UConfirmScreenInfoObject* ConfirmScreenInfoObject = NewObject<UConfirmScreenInfoObject>();

    ConfirmScreenInfoObject->ScreenTitle = InScreenTitle;
    ConfirmScreenInfoObject->ScreenMessage = InScreenMessage;

    FConfirmScreenButtonInfo NoButtonInfo;
    NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
    NoButtonInfo.ButtonTextToDisplay = Option1Text;

    FConfirmScreenButtonInfo YesButtonInfo;
    YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    YesButtonInfo.ButtonTextToDisplay = Option2Text;

    ConfirmScreenInfoObject->AvailableScreenButtons.Add(NoButtonInfo);
    ConfirmScreenInfoObject->AvailableScreenButtons.Add(YesButtonInfo);

    return ConfirmScreenInfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage, const FText& Option1Text, const FText& Option2Text)
{
    UConfirmScreenInfoObject* ConfirmScreenInfoObject = NewObject<UConfirmScreenInfoObject>();

    ConfirmScreenInfoObject->ScreenTitle = InScreenTitle;
    ConfirmScreenInfoObject->ScreenMessage = InScreenMessage;

    FConfirmScreenButtonInfo OKButtonInfo;
    OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    OKButtonInfo.ButtonTextToDisplay = Option1Text;

    FConfirmScreenButtonInfo CancelButtonInfo;
    CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
    CancelButtonInfo.ButtonTextToDisplay = Option2Text;

    ConfirmScreenInfoObject->AvailableScreenButtons.Add(OKButtonInfo);
    ConfirmScreenInfoObject->AvailableScreenButtons.Add(CancelButtonInfo);

    return ConfirmScreenInfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateCancelYesScreen(const FText& InScreenTitle, const FText& InScreenMessage, const FText& Option1Text, const FText& Option2Text)
{
    UConfirmScreenInfoObject* ConfirmScreenInfoObject = NewObject<UConfirmScreenInfoObject>();

    ConfirmScreenInfoObject->ScreenTitle = InScreenTitle;
    ConfirmScreenInfoObject->ScreenMessage = InScreenMessage;

    FConfirmScreenButtonInfo OKButtonInfo;
    OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
    OKButtonInfo.ButtonTextToDisplay = Option1Text;

    FConfirmScreenButtonInfo CancelButtonInfo;
    CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    CancelButtonInfo.ButtonTextToDisplay = Option2Text;

    ConfirmScreenInfoObject->AvailableScreenButtons.Add(OKButtonInfo);
    ConfirmScreenInfoObject->AvailableScreenButtons.Add(CancelButtonInfo);

    return ConfirmScreenInfoObject;
}

void UConfirmScreenWidget::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
    check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons); 

    CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle); 
    CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage); 

    // Check if Entry Box has Old Buttons Created Previously
    if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
    {
        // Clear Old Buttons Entry Box Has. Widget Type for Entry Box is Specified in Child Widget Blueprint
        DynamicEntryBox_Buttons->Reset<UOWCommonButtonBase>(
            [](UOWCommonButtonBase& ExistingButton)
            {
                ExistingButton.OnClicked().Clear();
            }
        ); 
    }

    check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());

    for (const FConfirmScreenButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableScreenButtons)
    {
        FDataTableRowHandle InputActionRowHandle;

        switch (AvailableButtonInfo.ConfirmScreenButtonType)
        {
        case EConfirmScreenButtonType::Confirmed:
        {
            InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultClickAction();
            break;
        }
        case EConfirmScreenButtonType::Cancelled:
        {
            InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
            break;
        }
        case EConfirmScreenButtonType::Closed:
        {
            InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
            break;
        }
        default:
            break;
        }

        UOWCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UOWCommonButtonBase>(); 
        AddedButton->SetButtonDisplayText(AvailableButtonInfo.ButtonTextToDisplay); 
        AddedButton->SetTriggeringInputAction(InputActionRowHandle);
        AddedButton->OnClicked().AddLambda(
            [ClickedButtonCallback, AvailableButtonInfo, this]()
            {
                ClickedButtonCallback(AvailableButtonInfo.ConfirmScreenButtonType);

                DeactivateWidget();
            }
        );
    }

    if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
    {
        // Set Focus on Last Button
        // So if there are 2 Buttons, One is Yes, One is No, Our Gamepad will Focus on No Button
        DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus(); 
    }

}
