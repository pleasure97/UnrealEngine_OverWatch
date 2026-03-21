// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "OWCommonListView.generated.h"

class UListEntryMappingDataAsset;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWCommonListView : public UCommonListView
{
	GENERATED_BODY()
	
protected:
	/* UListView */
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	/* UListView Ends */

private:
	/* UWidget Interface */
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	/* UWidget Interface Ends */

	UPROPERTY(EditAnywhere, Category = "List View Settings")
	UListEntryMappingDataAsset* ListEntryMappingDataAsset;
};
