// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OptionDataRegistry.generated.h"

class UListDataObjectCollection;
class UListDataObjectBase;
class UListDataObjectKeyRemap;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOptionDataRegistry : public UObject
{
	GENERATED_BODY()
	
public:
	// Called by Options Screen Right After Object of Type UOptionDataRegistry is Created 
	void InitOptionDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UListDataObjectCollection*>& GetRegisteredOptionTabCollections() const { return RegisteredOptionTabCollections; };

	TArray<UListDataObjectBase*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;

private:
	void FindChildListDataRecursively(UListDataObjectBase* InParentData, TArray<UListDataObjectBase*>& OutFoundChildListData) const;

	void InitGraphicCollectionTab();
	void InitAudioCollectionTab();
	void InitGameplayCollectionTab();
	void InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer);

	void AddKeyRemapDataToCategory(UListDataObjectCollection* InCategoryCollection);

	UPROPERTY(Transient)
	TArray<UListDataObjectCollection*> RegisteredOptionTabCollections;

	UPROPERTY(Transient)
	TMap<UListDataObjectKeyRemap*, FText> KeyRemapListDataObjectMap;
};
