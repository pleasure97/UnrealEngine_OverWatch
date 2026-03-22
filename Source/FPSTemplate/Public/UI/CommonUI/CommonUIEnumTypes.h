// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	OK, 
	YesNo, 
	NoYes,
	CancelYes,
	OKCancel,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed,
	Cancelled,
	Closed,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified, 
	DependencyModified,
	ResetToDefault,
};

UENUM(BlueprintType)
enum class EPlaybackDeviceType : uint8
{
	Default,
};

UENUM(BlueprintType)
enum class ESpatialAudioType : uint8
{
	SystemSettings,
	DolbyAtmosForHeadphones,
};

UENUM(BlueprintType)
enum class EAudioMixType : uint8
{
	Default,
	Speakers,
	Headphones,
	NightMode,
	StudioReference
};