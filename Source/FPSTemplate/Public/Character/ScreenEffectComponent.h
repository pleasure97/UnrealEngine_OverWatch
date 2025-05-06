// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScreenEffectComponent.generated.h"

class UPostProcessComponent; 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTEMPLATE_API UScreenEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Integrated Material Parameter Collection 
	UPROPERTY(EditAnywhere, Category = "Scene Effects")
	TObjectPtr<UMaterialParameterCollection> MaterialParameterCollection; 

	UPROPERTY(EditAnywhere, Category = "Screen Effects")
	TArray<UMaterialInterface*> PostProcessMaterials; 

	void SetScalarParameterValue(FName ScalaraParameterName, float ScalarParameterValue);

protected:
	virtual void OnRegister() override;

private:
	UPROPERTY()
	TObjectPtr<UPostProcessComponent> PostProcessComponent; 

	void ApplyPostProcessMaterials(); 
};
