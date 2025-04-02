// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "TeamInterface.generated.h"

template <typename InterfaceType> class TScriptInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTeamIndexChangedDelegate, UObject*, ObjectChangingTeam, int32, OldTeamID, int32, NewTeamID); 

inline int32 GenericTeamIdToInteger(FGenericTeamId ID)
{
	return (ID == FGenericTeamId::NoTeam) ? INDEX_NONE : (int32)ID; 
}

inline FGenericTeamId IntegerToGenericTeamId(int32 ID)
{
	return (ID == INDEX_NONE) ? FGenericTeamId::NoTeam : FGenericTeamId((uint8)ID); 
}

// This class does not need to be modified.
UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class UTeamInterface : public UGenericTeamAgentInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSTEMPLATE_API ITeamInterface : public IGenericTeamAgentInterface 
{
	GENERATED_BODY()

public:
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() { return nullptr; }

	static void BroadcastTeamChanged(TScriptInterface<ITeamInterface> TeamInterface, FGenericTeamId OldTeamID, FGenericTeamId NewTeamID); 

	FOnTeamIndexChangedDelegate& GetTeamChangedDelegate()
	{
		FOnTeamIndexChangedDelegate* OnTeamIndexChangeDelegate = GetOnTeamIndexChangedDelegate(); 
		check(OnTeamIndexChangeDelegate); 
		return *OnTeamIndexChangeDelegate;
	}
};
