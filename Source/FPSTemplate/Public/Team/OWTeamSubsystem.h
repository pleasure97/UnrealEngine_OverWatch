// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "OWTeamSubsystem.generated.h"

class AOWTeamPublicInfo;
class AOWTeamPrivateInfo;
class AOWTeamInfoBase;
class AOWPlayerState; 
class UOWTeamDisplayAsset; 

USTRUCT()
struct FOWTeamTrackingInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<AOWTeamPublicInfo> PublicInfo = nullptr; 

	UPROPERTY()
	TObjectPtr<AOWTeamPrivateInfo> PrivateInfo = nullptr; 

	UPROPERTY()
	TObjectPtr<UOWTeamDisplayAsset> DisplayAsset = nullptr; 

	void SetTeamInfo(AOWTeamInfoBase* Info); 
	void RemoveTeamInfo(AOWTeamInfoBase* Info); 
};

UENUM(BlueprintType)
enum class EOWTeamComparison : uint8
{
	OnSameTeam, 
	DifferentTeams, 
	InvalidArgument
};

UCLASS()
class FPSTEMPLATE_API UOWTeamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	bool RegisterTeamInfo(AOWTeamInfoBase* TeamInfo); 
	bool UnregisterTeamInfo(AOWTeamInfoBase* TeamInfo); 

	const AOWPlayerState* FindPlayerStateFromActor(const AActor* PossibleTeamActor) const; 

	int32 FindTeamFromObject(const UObject* TestObject) const; 

	EOWTeamComparison CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const; 

	bool CanCauseDamage(const UObject* Instigator, const UObject* Target, bool bAllowDamageToSelf = true) const; 
private:
	UPROPERTY()
	TMap<int32, FOWTeamTrackingInfo> TeamMap; 
};
