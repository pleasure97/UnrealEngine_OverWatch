// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenWidget.h"
#include "LoadingScreenSubsystem.h"

void ULoadingScreenWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Get Loading Screen Subsystem which is type of Game Instance Subsystem
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			ULoadingScreenSubsystem* LoadingScreenSubsystem = GameInstance->GetSubsystem<ULoadingScreenSubsystem>(); 
			if (LoadingScreenSubsystem)
			{
				// Bind Loading Screen Subsystem's Loading Screen Widget Changed
				LoadingScreenSubsystem->OnLoadingScreenWidgetChanged.AddDynamic(this, &ULoadingScreenWidget::OnLoadingScreenWidgetChanged); 

				// Call Once
				OnLoadingScreenWidgetChanged(LoadingScreenSubsystem->GetLoadingScreenWidget()); 
			}
		}
	}
}

void ULoadingScreenWidget::OnLoadingScreenWidgetChanged(TSubclassOf<UUserWidget> NewWidgetClass)
{
	// Save Broadcasted New Widget Class
	ContentWidgetClass = NewWidgetClass; 

	// Check New Widget Class is Valid 
	if (IsValid(ContentWidgetClass))
	{
		UUserWidget* CreatedWidget = CreateWidget(GetOwningPlayer(), ContentWidgetClass); 
	}
	// If not, Create Default Loading Screen Widget 
	else
	{
		UUserWidget* CreatedWidget = CreateWidget(GetOwningPlayer(), DefaultLoadingScreenWidgetClass); 
	}
}


