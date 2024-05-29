// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

#include "ShaderCompiler.h"
#include "VectorUtil.h"
#include "DynamicMesh/DynamicMesh3.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	GlobalOriginalLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
	OriginalDirection = (GlobalTargetLocation - GlobalOriginalLocation).GetSafeNormal();
	// DirectionWeight = 1;
	// DirectionState = false;

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(ActiveTriggers > 0)
	{
	    if (HasAuthority())
    	{
    		MoveStartToEnd(DeltaSeconds);
    	}
	}
}

// State변수를 사용해서 시작과 끝일때 true, false인 것으로 구현해도 되지만
// 좀더 쉬운 방법은 도착위치에 다다랐을때 시작위치와 도착위치를 바꿔주면 된다.
// 즉 Swap 함수 방법을 사용하면 됨!!
void AMovingPlatform::MoveStartToEnd(float DeltaSeconds)
{
	FVector CurrentLocation = GetActorLocation();
	FVector Direction = (GlobalTargetLocation - CurrentLocation).GetSafeNormal();

	if (FVector::PointsAreNear(CurrentLocation, GlobalTargetLocation, 1))
	{
		FVector temp = GlobalOriginalLocation;
		GlobalOriginalLocation = GlobalTargetLocation;
		GlobalTargetLocation = temp;
	}

	// if(!DirectionState && FVector::PointsAreNear(CurrentLocation, GlobalTargetLocation, 1))
	// {
	// 	DirectionWeight = -1;
	// 	DirectionState = true;
	// }
	//
	// if(DirectionState && FVector::PointsAreNear(CurrentLocation, GlobalOriginalLocation, 1))
	// {
	// 	DirectionWeight = 1;
	// 	DirectionState = false;
	// }

	// SetActorLocation(CurrentLocation + DirectionWeight * Direction * Lambda * DeltaSeconds);
	SetActorLocation(CurrentLocation + Direction * Lambda * DeltaSeconds);
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if(ActiveTriggers > 0)
	{
		ActiveTriggers--;
	}
}
