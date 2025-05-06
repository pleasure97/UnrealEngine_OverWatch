// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ScreenEffectComponent.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void UScreenEffectComponent::OnRegister()
{
	Super::OnRegister();

	PostProcessComponent = NewObject<UPostProcessComponent>(GetOwner()); 
	PostProcessComponent->SetupAttachment(GetOwner()->GetRootComponent()); 
	PostProcessComponent->bUnbound = true; 
	PostProcessComponent->RegisterComponent(); 

	ApplyPostProcessMaterials();
}

void UScreenEffectComponent::ApplyPostProcessMaterials()
{
	if (PostProcessMaterials.IsEmpty() || !PostProcessComponent)
	{
		return;
	}

	PostProcessComponent->Settings.WeightedBlendables.Array.Empty(); 

	for (UMaterialInterface* PostProcessMaterial : PostProcessMaterials)
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(PostProcessMaterial, this); 
		PostProcessComponent->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.f, MID)); 
	}
}

void UScreenEffectComponent::SetScalarParameterValue(FName ScalarParameterName, float ScalarParameterValue)
{
	if (!MaterialParameterCollection)
	{
		return; 
	}

	if (UMaterialParameterCollectionInstance* MPCInstance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection))
	{
		MPCInstance->SetScalarParameterValue(ScalarParameterName, ScalarParameterValue); 
	}
}
