// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/InteractiveAgent.h"

AInteractiveAgent::AInteractiveAgent()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AInteractiveAgent::AddInteractionOptions(const FInteractionQuery& InteractionQuery, FInteractionOptionCollector& InteractionOptionCollector)
{
	InteractionOptionCollector.Add(InteractionOption); 
}
