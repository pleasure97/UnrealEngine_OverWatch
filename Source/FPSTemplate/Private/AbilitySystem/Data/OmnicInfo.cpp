// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/OmnicInfo.h"

FOmnicClassDefaultInfo UOmnicInfo::GetOmnicDefaultInfo(EOmnicClass OmnicClass)
{
	return OmnicClassInformation.FindChecked(OmnicClass); 
}
