// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractiveAgent/InteractiveAgentManager.h"
#include "Kismet/GameplayStatics.h"

UInteractiveAgentSubsystem* UInteractiveAgentManager::GetInteractiveAgentSubsystem() const
{
	if (UGameInstance* OwningGameInstance = UGameplayStatics::GetGameInstance(this))
	{
		if (UInteractiveAgentSubsystem* InteractiveAgentSubsystem = OwningGameInstance->GetSubsystem<UInteractiveAgentSubsystem>())
		{
			return InteractiveAgentSubsystem;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Interactive Agent Subsystem is Not Valid in UInteractiveAgentManager::GetInteractiveAgentSubsystem()"));
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Get Game Instance inUInteractiveAgentManager::GetInteractiveAgentSubsystem()"));
		return nullptr;
	}
}


