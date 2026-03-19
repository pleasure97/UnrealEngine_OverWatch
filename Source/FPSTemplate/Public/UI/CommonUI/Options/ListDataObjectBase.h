// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ListDataObjectBase.generated.h"

#define LIST_DATA_ACCESORS(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; }	\
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UListDataObjectBase : public UObject
{
	GENERATED_BODY()
	
public:
	LIST_DATA_ACCESORS(FName, DataID);
	LIST_DATA_ACCESORS(FText, DataDisplayName);
	LIST_DATA_ACCESORS(FText, DataDescriptionText);
	LIST_DATA_ACCESORS(FText, DisabledText);
	LIST_DATA_ACCESORS(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage);
	LIST_DATA_ACCESORS(UListDataObjectBase*, ParentData);

	void InitDataObject();

	// Empty in Base Class. Child Class ListDataObjectCollection Should Override it
	// The Function Should Return All Children Data Tab Has
	virtual TArray<UListDataObjectBase*> GetChildListData() const { return TArray<UListDataObjectBase*>(); }
	virtual bool HasChildListData() const { return false; }

protected:
	// Child Classes Should Override it to Handle Initialized Needed Accordingly
	virtual void OnDataObjectInitialized(); 

private:
	FName DataID;
	FText DataDisplayName;
	FText DataDescriptionText;
	FText DisabledText;

	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UListDataObjectBase* ParentData;
};
