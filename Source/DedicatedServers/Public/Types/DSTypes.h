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
	PreMatch UMETA(DisplayName = "Pre Match"), 
	Match UMETA(DisplayName = "Match"), 
	PostMatch UMETA(DisplayName = "Post Match"), 
	None UMETA(DisplayName = "None")
};

UENUM()
enum class EMatchStatus : uint8
{
	WaitingForPlayers, 
	PreMatch, 
	Match, 
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