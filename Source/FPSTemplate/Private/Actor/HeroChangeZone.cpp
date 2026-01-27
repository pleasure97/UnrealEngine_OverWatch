// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/HeroChangeZone.h"
#include "Components/BoxComponent.h"

AHeroChangeZone::AHeroChangeZone()
{
	Box = CreateDefaultSubobject<UBoxComponent>("Box"); 
	Box->SetupAttachment(GetRootComponent());
	Box->OnComponentBeginOverlap.AddDynamic(this, &AHeroChangeZone::OnHeroChangeZoneOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AHeroChangeZone::OnHeroChangeZoneEndOverlap);
}

void AHeroChangeZone::OnHeroChangeZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnOverlap(OtherActor);
}

void AHeroChangeZone::OnHeroChangeZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnEndOverlap(OtherActor);
}
