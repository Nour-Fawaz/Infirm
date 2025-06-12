// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	TargetLocation = OriginalLocation;

	OriginalRotation = GetOwner()->GetActorRotation();
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveActor(DeltaTime);

	RotateActor(DeltaTime);
}


void UMover::MoveActor(float DeltaTime)
{
	//FVector TargetLocation = OriginalLocation; // by default actor moves back to original location

	if (ShouldMove)
	{
		TargetLocation = OriginalLocation + MoveOffset; //otherwise update the target location to be wanted target location

	}

	FVector CurrentLocation = GetOwner()->GetActorLocation();
	float Speed = MoveOffset.Length() / MoveTime; // diff between target and original location is move lcoation

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(NewLocation);
}

void UMover::RotateActor(float DeltaTime)
{
	FRotator TargetRotation = OriginalRotation;
	if (ShouldRotate)
	{
		TargetRotation = OriginalRotation + RotationOffset;
	}

	FRotator NewRotation = FMath::RInterpConstantTo(GetOwner()->GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
	GetOwner()->SetActorRotation(NewRotation);
}

bool UMover::GetShouldRotate() const
{
	return ShouldRotate;
}

void UMover::SetShouldMove(bool ShouldMoveIn)
{
	ShouldMove = ShouldMoveIn;
}

void UMover::SetShouldRotate(bool ShouldRotateIn)
{
	ShouldRotate = ShouldRotateIn;
}