// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFIRM_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Setters and getters
	UFUNCTION(BlueprintCallable)
	void SetShouldMove(bool ShouldMoveIn);

	UFUNCTION(BlueprintCallable)
	void SetShouldRotate(bool ShouldRotateIn);

	UFUNCTION(BlueprintCallable)
	bool GetShouldRotate() const;


private:

	// Movement proeprties
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector MoveOffset;//how far mover should move when activated
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveTime = 4;//amount of time it takes to move to target
	bool ShouldMove = false; //boolean to activate mover
	FVector OriginalLocation;
	FVector TargetLocation;


	// Rotation proeprties
	bool ShouldRotate = false;
	FRotator OriginalRotation;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationOffset;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	double RotationSpeed = 0;


	UFUNCTION(BlueprintCallable)
	void MoveActor(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void RotateActor(float DeltaTime);

		
};
