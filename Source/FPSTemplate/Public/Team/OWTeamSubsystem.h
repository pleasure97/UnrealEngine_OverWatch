// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override; 
	virtual void Deinitialize() override;

	bool RegisterTeamInfo(AOWTeamInfoBase* TeamInfo); 
	bool UnregisterTeamInfo(AOWTeamInfoBase* TeamInfo); 

	const AOWPlayerState* FindPlayerStateFromActor(const AActor* PossibleTeamActor) const; 

	bool ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamIndex); 
	int32 FindTeamFromObject(const UObject* TestObject) const; 

	EOWTeamComparison CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const; 
	EOWTeamComparison CompareTeams(const UObject* A, const UObject* B) const; 

	bool CanCauseDamage(const UObject* Instigator, const UObject* Target, bool bAllowDamageToSelf = true) const; 

	void AddTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount); 
	void RemoveTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount); 

	int32 GetTeamTagStackCount(int32 TeamId, FGameplayTag Tag) const;
private:
	UPROPERTY()
	TMap<int32, FOWTeamTrackingInfo> TeamMap; 
};
