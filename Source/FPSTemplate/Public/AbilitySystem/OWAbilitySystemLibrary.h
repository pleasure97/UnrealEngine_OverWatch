// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OWAbilitySystemLibrary.generated.h"

struct FWidgetControllerParams; 
class AOWHUD; 
class UOverlayWidgetController; 
class UHeroInfo; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/*
	 * Widget Controller 
	 */

	UFUNCTION(BlueprintCallable, Category="OWAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AOWHUD*& OutOWHUD); 
	
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject); 

	/*
	 * Hero Info Defaults 
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EHeroName HeroName, UAbilitySystemComponent* ASC, float Level=1.f); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static void GiveDefaultAbilities(const UObject* WorldContextObject, EHeroName HeroName, UAbilitySystemComponent* ASC); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static UHeroInfo* GetHeroInfo(const UObject* WorldContextObject); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static EHeroName GetHeroName(const UObject* WorldContextObject);
};
