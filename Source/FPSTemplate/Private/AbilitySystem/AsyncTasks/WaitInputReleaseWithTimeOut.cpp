// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitInputReleaseWithTimeOut.h"

void UWaitInputReleaseWithTimeOut::Activate()
{
	Super::Activate(); 

	if (MaxHoldTime > 0.f && GetWorld())
	{
		StartTime = GetWorld()->GetTimeSeconds(); 
		GetWorld()->GetTimerManager().SetTimer(TimeOutHandle, this, &UWaitInputReleaseWithTimeOut::OnTimeOutReached, MaxHoldTime, false); 
	}

	OnRelease.AddDynamic(this, &UWaitInputReleaseWithTimeOut::OnActualRelease); 
}

UWaitInputReleaseWithTimeOut* UWaitInputReleaseWithTimeOut::WaitInputReleaseWithTimeOut(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InMaxHoldTime)
{
	UWaitInputReleaseWithTimeOut* Task = NewAbilityTask<UWaitInputReleaseWithTimeOut>(OwningAbility, TaskInstanceName); 
	
	Task->MaxHoldTime = InMaxHoldTime; 

	return Task;
}

void UWaitInputReleaseWithTimeOut::OnActualRelease(float TimeHeld)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeOutHandle); 

		float HoldTime = GetWorld()->GetTimeSeconds() - StartTime; 

		OnReleaseOrTimeOut.Broadcast(HoldTime); 
		EndTask(); 
	}
}

void UWaitInputReleaseWithTimeOut::OnTimeOutReached()
{
	OnReleaseOrTimeOut.Broadcast(MaxHoldTime); 
	EndTask(); 
}
