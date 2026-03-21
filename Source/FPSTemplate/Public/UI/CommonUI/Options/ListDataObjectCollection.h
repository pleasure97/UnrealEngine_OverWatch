// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/Options/ListDataObjectBase.h"
#include "ListDataObjectCollection.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UListDataObjectCollection : public UListDataObjectBase
{
	GENERATED_BODY()
	
public:
	void AddChildListData(UListDataObjectBase* InChildListData);

	/* UListDataObjectBase */
	virtual TArray<UListDataObjectBase*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;
	/* UListDataObjectBase Ends */

private:
	UPROPERTY(Transient)
	TArray<UListDataObjectBase*> ChildListDataArray;
};
