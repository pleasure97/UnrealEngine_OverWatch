// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EffectActor.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystemComponent.h"


AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false; 

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh"); 
	SetRootComponent(Mesh); 

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere"); 
	Sphere->SetupAttachment(GetRootComponent()); 
}

void AEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UOWAttributeSet* OWAttributeSet = Cast<UOWAttributeSet>(ASInterface->GetAbilitySystemComponent()->GetAttributeSet(UOWAttributeSet::StaticClass())); 

		UOWAttributeSet* MutableOWAttributeSet = const_cast<UOWAttributeSet*>(OWAttributeSet); 

		// TODO : 250.f is literal 
		MutableOWAttributeSet->SetHealth(OWAttributeSet->GetHealth() + 250.f); 
		
		// TODO : Just hide and respawn itself. 
		Destroy(); 
	}
}

void AEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEffectActor::EndOverlap);
}

