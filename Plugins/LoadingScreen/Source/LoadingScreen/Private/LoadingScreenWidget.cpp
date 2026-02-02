// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenWidget.h"
#include "LoadingScreenSubsystem.h"
#include "Components/PanelWidget.h"

void ULoadingScreenWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Get Loading Screen Subsystem which is type of Game Instance Subsystem
	ULoadingScreenSubsystem* LoadingScreenSubsystem = GetGameInstance()->GetSubsystem<ULoadingScreenSubsystem>();
	if (!IsValid(LoadingScreenSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("Loading Screen Subsystem is Not Valid in ULoadingScreenWidget::NativeConstruct()"));
		return;
	}

	// Bind Loading Screen Subsystem's Loading Screen Widget Changed
	LoadingScreenSubsystem->OnLoadingScreenWidgetChanged.AddDynamic(this, &ULoadingScreenWidget::OnLoadingScreenWidgetChanged);

	// Call Once
	OnLoadingScreenWidgetChanged(LoadingScreenSubsystem->GetLoadingScreenWidget());
}

void ULoadingScreenWidget::OnLoadingScreenWidgetChanged(TSubclassOf<UUserWidget> NewWidgetClass)
{
	// Save Broadcasted New Widget Class
	ContentWidgetClass = IsValid(NewWidgetClass) ? NewWidgetClass : DefaultLoadingScreenWidgetClass;

	// Check if Content Panel Widget is Valid 
	if (!IsValid(PanelWidget_Content))
	{
		return;
	}

	// Remove All Previously Added Child Widgets
	PanelWidget_Content->ClearChildren();

	if (IsValid(ContentWidgetClass))
	{
		// Create New Content Widget and Add it to Panel Widget
		UUserWidget* NewContentWidget = CreateWidget<UUserWidget>(this, ContentWidgetClass); 
		if (IsValid(NewContentWidget))
		{
			PanelWidget_Content->AddChild(NewContentWidget);
		}
	}
}


