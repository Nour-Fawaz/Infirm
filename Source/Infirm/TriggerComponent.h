// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "FirstPersonPlayer.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) //to create component in BP
class INFIRM_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();

protected:
	void BeginPlay();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* NewMover);

	UFUNCTION(BlueprintCallable)
	FName GetTriggerActorName() const;


private:
	UPROPERTY(EditAnywhere, Category = "KeyName")
	FName TriggerActor;

	UMover* Mover;

	UFUNCTION(BlueprintCallable)
	AActor* GetTriggerActor() const; //doesnt change anything hence make it const

	//Player
	UPROPERTY()
	AFirstPersonPlayer* FPP;
	UPROPERTY()
	class AFirstPersonController* FPC;
};
