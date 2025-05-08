// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacterBase.h"
#include "Interface/OmnicInterface.h"
#include "OWOmnic.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWOmnic : public AOWCharacterBase, public IOmnicInterface
{
	GENERATED_BODY()
	
public:
	AOWOmnic(); 

	/* Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override; 
	virtual void Die(const FVector& DeathImpulse) override; 
	/* End Combat Interface */

	/* Omnic Interface */
	virtual EOmnicClass GetOmnicClass_Implementation() override; 
	/* End Omnic Interface*/

	void SetOmnicClass(EOmnicClass InClass) { OmnicClass = InClass; }
	void SetLevel(int32 InLevel) { Level = InLevel; }
protected:
	virtual void BeginPlay() override; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	EOmnicClass OmnicClass = EOmnicClass::Training; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	
};
