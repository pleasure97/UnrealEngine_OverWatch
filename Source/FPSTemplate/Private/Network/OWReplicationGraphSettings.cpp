// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/OWReplicationGraphSettings.h"
#include "Network/OWReplicationGraph.h"

UOWReplicationGraphSettings::UOWReplicationGraphSettings()
{
	CategoryName = TEXT("Game"); 
	DefaultReplicationGraphClass = UOWReplicationGraph::StaticClass();
}
