// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ScreenEffectComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"


void UScreenEffectComponent::ApplyPostProcessMaterials(UCameraComponent* CameraComponent)
{
	if (PostProcessMaterials.IsEmpty() || !MaterialInstanceDynamics.IsEmpty())
	{
		return;
	}

	for (UMaterialInterface* PostProcessMaterial : PostProcessMaterials)
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);
		if (!MID)
		{
			return; 
		}
		MaterialInstanceDynamics.Add(MID); 
		CameraComponent->AddOrUpdateBlendable(MID, 1.f); 
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
