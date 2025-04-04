// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/OWTeamDisplayAsset.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/MeshComponent.h"
#include "NiagaraComponent.h"

void UOWTeamDisplayAsset::ApplyToMaterial(UMaterialInstanceDynamic* DynamicMaterialInstance)
{
	if (DynamicMaterialInstance)
	{
		for (const auto& ColorParameter : ColorParameters)
		{
			DynamicMaterialInstance->SetVectorParameterValue(ColorParameter.Key, FVector(ColorParameter.Value)); 
		}
	}
}

void UOWTeamDisplayAsset::ApplyToMeshComponent(UMeshComponent* MeshComponent)
{
	if (MeshComponent)
	{
		for (const auto& ColorParameter : ColorParameters)
		{
			MeshComponent->SetVectorParameterValueOnMaterials(ColorParameter.Key, FVector(ColorParameter.Value)); 
		}

		const TArray<UMaterialInterface*> MaterialInterfaces = MeshComponent->GetMaterials(); 
		for (int32 MaterialIndex = 0; MaterialIndex < MaterialInterfaces.Num(); ++MaterialIndex)
		{
			if (UMaterialInterface* MaterialInterface = MaterialInterfaces[MaterialIndex])
			{
				UMaterialInstanceDynamic* DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(MaterialInterface); 
				if (!DynamicMaterialInstance)
				{
					DynamicMaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(MaterialIndex); 
				}
			}
		}
	}
}

void UOWTeamDisplayAsset::ApplyToNiagaraComponent(UNiagaraComponent* NiagaraComponent)
{
	if (NiagaraComponent)
	{
		for (const auto& ColorParameter : ColorParameters)
		{
			NiagaraComponent->SetVariableLinearColor(ColorParameter.Key, ColorParameter.Value); 
		}
	}
}

void UOWTeamDisplayAsset::ApplyToActor(AActor* TargetActor, bool bIncludeChildActors)
{
	if (TargetActor != nullptr)
	{
		TargetActor->ForEachComponent(bIncludeChildActors, [this](UActorComponent* InComponent)
			{
				if (UMeshComponent* MeshComponent = Cast<UMeshComponent>(InComponent))
				{
					ApplyToMeshComponent(MeshComponent); 
				}
				else if (UNiagaraComponent* NiagaraComponent = Cast<UNiagaraComponent>(InComponent))
				{
					ApplyToNiagaraComponent(NiagaraComponent); 
				}
			});
	}
}
