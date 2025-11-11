// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/API/InteractiveAPIData.h"

FString UInteractiveAPIData::GetAPIModel()
{
	return APIModel;
}

FString UInteractiveAPIData::GetAPIUrl()
{
	return APIUrl; 
}

FString UInteractiveAPIData::GetAPIKey()
{
	return APIKey; 
}
