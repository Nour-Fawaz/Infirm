// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonController.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	UKismetSystemLibrary::PrintString(this, TEXT("Trigger Comp alive"), true, true, FColor::Blue, 5.0f);

	
	//get FPC 
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	if (FPC)
	{
		FPP = Cast<AFirstPersonPlayer>(FPC->GetPawn());

	}

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); //call parent tick component func (Parent is box collision component)

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (AActor* CurrentActor : OverlappingActors)
	{
		if (CurrentActor->ActorHasTag(TriggerActor)) //MAKE IT S.T. ITS CHECKING EQUIPPED ITEM ACTOR TAGS AND REMOVE TRIGGERACTOR TAG FROM PLAYER    
		{
			//UE_LOG(LogTemp, Display, TEXT("Actor has trigger actor"));
			Mover->SetShouldMove(true);
			Mover->SetShouldRotate(true);
			if (TriggerActor == "1234")
			{
				return;
			}
			if (FPP->ItemEquipped)
			{
				FPP->RemoveEquippedItem();
			}

			FPC->RemoveFromInventory(TriggerActor);
			

		}
		else 
		{
			Mover->SetShouldMove(false);
			Mover->SetShouldRotate(false);
		}
	}
	
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

FName UTriggerComponent::GetTriggerActorName() const
{
	return TriggerActor;
}

AActor* UTriggerComponent::GetTriggerActor() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors); // populate list with overlapping actors

	// iterate through all actors in TArray
	for (AActor* CurrentActor : OverlappingActors)
	{
		if (CurrentActor->ActorHasTag(TriggerActor)) // if the current actor has the specified tag
		{
			return CurrentActor; // return CurrentActor if it is key to door
		}
	}
	return nullptr;
}
