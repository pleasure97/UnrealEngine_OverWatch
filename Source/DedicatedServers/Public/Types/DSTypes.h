#pragma once

#include "DSTypes.generated.h"

UENUM(BlueprintType)
enum class ECountTimerState : uint8
{
	NotStarted UMETA(DisplayName = "Not Started"), 
	Started UMETA(DisplayName = "Started"), 
	Paused UMETA(DisplayName = "Paused"), 
	Stopped UMETA(DisplayName = "Stopped")
};

UENUM(BlueprintType)
enum class ECountTimerDirection : uint8
{
	Countdown UMETA(DisplayName = "Countdown"), 
	Countup UMETA(DisplayName = "Countup")
};

UENUM(BlueprintType)
enum class ECountTimerType : uint8
{
	LobbyCountdown UMETA(DisplayName = "Lobby Countdown"), 
	LobbyCountup UMETA(DisplayName = "Lobby Countup"), 
	FirstHeroSelection UMETA(DisplayName = "First Hero Selection"), 
	FirstMatchPreparation UMETA(DisplayName = "First Match Preparation"),
	FirstMatch UMETA(DisplayName = "First Match"), 
	SwitchInning UMETA(DisplayName = "Switch Inning"),
	SecondHeroSelection UMETA(DisplayName = "Second Hero Selection"),
	SecondMatchPreparation UMETA(DisplayName = "Second Match Preparation"),
	SecondMatch UMETA(DisplayName = "Second Match"),
	PostMatch UMETA(DisplayName = "Post Match"), 
	None UMETA(DisplayName = "None")
};

UENUM()
enum class EMatchStatus : uint8
{
	WaitingForPlayers, 
	FirstHeroSelection, 
	FirstMatchPreparation,
	FirstMatch, 
	SwitchInning, 
	SecondHeroSelection, 
	SecondMatchPreparation, 
	SecondMatch,
	PostMatch, 
	SeamlessTravelling
};

UENUM()
enum class ELobbyStatus : uint8
{
	WaitingForPlayers, 
	PreparingSeamlessTravel, 
	SeamlessTravelling
};


USTRUCT(BlueprintType)
struct FCountTimerHandle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ECountTimerState State = ECountTimerState::NotStarted; 

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ECountTimerDirection Direction = ECountTimerDirection::Countdown; 

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ECountTimerType Type = ECountTimerType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CountTime = 0.f; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CountUpdateInterval = 1.f; 

	FTimerHandle TimerFinishedHandle{}; 
	FTimerHandle TimerUpdateHandle{}; 

	FTimerDelegate TimerFinishedDelegate{}; 
	FTimerDelegate TimerUpdateDelegate{}; 
}; 

inline bool operator==(const FCountTimerHandle& lhs, const FCountTimerHandle& rhs)
{
	return (lhs.Type == rhs.Type) && (lhs.Direction == rhs.Direction); 
}