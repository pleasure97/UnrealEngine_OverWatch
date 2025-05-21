#include "UI/Widget/CombatLog.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCombatLog::ShowCombatLog(ECombatLogType CombatLogType, const FString& PlayerName)
{
	check(!IconMap.IsEmpty()); 

	switch (CombatLogType)
	{
	case ECombatLogType::Kill:
	{
		Border_CombatLog->SetBrushColor(CombatLogColors::Red);
		if (UTexture2D** FoundIcon = IconMap.Find(ECombatLogType::Kill))
		{
			Image_CombatLog->SetBrushFromTexture(*FoundIcon, true);
		}
		TextBlock_Username->SetText(FText::FromString(PlayerName));
		TextBlock_KillLog->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	case ECombatLogType::Death:
	{
		Border_CombatLog->SetBrushColor(CombatLogColors::Gray); 
		if (UTexture2D** FoundIcon = IconMap.Find(ECombatLogType::Death))
		{
			Image_CombatLog->SetBrushFromTexture(*FoundIcon, true);
		}
		TextBlock_Username->SetText(FText::FromString(PlayerName));
		TextBlock_KillLog->SetVisibility(ESlateVisibility::Visible);
		break;
	}
	}
}