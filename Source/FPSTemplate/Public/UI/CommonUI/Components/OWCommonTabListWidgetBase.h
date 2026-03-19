// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "OWCommonTabListWidgetBase.generated.h"

class UOWCommonButtonBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class FPSTEMPLATE_API UOWCommonTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName); 

private:
	/* UWidget Interface */
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	/* UWidget Interface Ends */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common UI Tab List Settings", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10"))
	int32 DebugEditorPreviewTabCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common UI Tab List Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UOWCommonButtonBase> TabButtonEntryWidgetClass;
};
