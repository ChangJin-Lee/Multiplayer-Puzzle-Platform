// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"

#include "MovingPlatform.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerVolume.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
	if(!ensure(TriggerVolume != nullptr)) return;
	
	RootComponent = TriggerVolume;
	
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);

	for(AMovingPlatform* Platform: PlatformsToTrigger)
	{
		if(Platform != nullptr)
		{
			Platform->RemoveActiveTrigger();
		}
	}
	
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for(AMovingPlatform* Platform: PlatformsToTrigger)
	{
		if(Platform != nullptr)
		{
			Platform->AddActiveTrigger();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Activated"));
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for(AMovingPlatform* Platform: PlatformsToTrigger)
	{
		if(Platform != nullptr)
		{
			Platform->RemoveActiveTrigger();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("DeActivated"));
}
