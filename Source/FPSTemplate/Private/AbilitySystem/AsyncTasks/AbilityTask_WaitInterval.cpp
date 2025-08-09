// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/AbilityTask_WaitInterval.h"

UAbilityTask_WaitInterval* UAbilityTask_WaitInterval::WaitInterval(UGameplayAbility* OwningAbility, float IntervalSeconds, float DurationSeconds)
{
	UAbilityTask_WaitInterval* MyObj = NewAbilityTask<UAbilityTask_WaitInterval>(OwningAbility); 
	MyObj->IntervalSeconds = FMath::Max(0.001f, IntervalSeconds);
	MyObj->RemainingTime = DurationSeconds;

	return MyObj;
}

void UAbilityTask_WaitInterval::Activate()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(WaitIntervalTimerHandle, this, &UAbilityTask_WaitInterval::HandleWaitInterval, IntervalSeconds, true);
	}
}

void UAbilityTask_WaitInterval::OnDestroy(bool bInOwnerFinished)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(WaitIntervalTimerHandle); 
	}

	Super::OnDestroy(bInOwnerFinished); 
}

void UAbilityTask_WaitInterval::HandleWaitInterval()
{
	if (RemainingTime >= 0.f)
	{
		RemainingTime -= IntervalSeconds;
		if (RemainingTime < 0.f)
		{
			OnWaitInterval.Broadcast();
			EndTask();
			return;
		}
	}

	OnWaitInterval.Broadcast();
}
