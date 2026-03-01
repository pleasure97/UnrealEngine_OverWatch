// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/OWMessageTypes.h"
#include "OWUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController); 

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController; 

	virtual void SetChildWidgetControllers();

	template<typename ClassType>
	void RegisterGamePhaseListener(FGameplayMessageListenerHandle& Handle, FGameplayTag Tag, ClassType* Object, void (ClassType::* Func)(FGameplayTag, const FOWVerbMessage&))
	{
		UGameplayMessageSubsystem& Subsystem = UGameplayMessageSubsystem::Get(Object);
		Handle = Subsystem.RegisterListener<FOWVerbMessage>(Tag, Object, Func);
	}

	void UnregisterGamePhaseListener(FGameplayMessageListenerHandle& Handle);
};
