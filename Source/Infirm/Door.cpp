// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "Misc/App.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "DisplayWidget.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonController.h"


ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	BoxComp->SetupAttachment(StaticMesh);

}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	//set actor rotation upon spawn
	OriginalRotation = this->GetActorRotation();

	//bind events
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OverlapBegin); 
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ADoor::OverlapEnd);

	//get FPP and FPC
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC);
	if (APC)
	{
		FPP = Cast<AFirstPersonPlayer>(APC->GetPawn());
	}
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//if actor is opening adjust rotation
	if (Opening)
	{
		TargetRotation = OriginalRotation + RotationOffset;
		FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
		SetActorRotation(NewRotation);
	}
}

/*
* function: called by player to open door
*/
void ADoor::OpenDoor()
{
	if (!bLocked)
	{
		Opening = true;
		DestroyAllWidgets();
		if (OpenDoorSoundEffect)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OpenDoorSoundEffect, GetActorLocation());
		}
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (FPP && FPP->ActorHasTag(Passkey)) 
		{
			FPC->RemoveFromInventory(Passkey); //if player uses key remove from inventory
			this->Tags.Remove(FName("LockedDoor")); //door no longer locked
		}
	}
}

/*
* function: destroys active widgets related to Door
*/
void ADoor::DestroyAllWidgets()
{
	if (LockedDoorWidget)
	{
		LockedDoorWidget->RemoveFromParent(); //remove widget from player viewport
		LockedDoorWidget = nullptr;
	}
}

void ADoor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if the player has the key unlock the door
	if (FPC->InInventory(Passkey))
	{
		UE_LOG(LogTemp, Display, TEXT("Actor has key"));
		DestroyAllWidgets();
		bLocked = false;
		
	}
	else
	{
		if (LockedDoorWidget)
		{
			return; //already showing widget
		}
		AFirstPersonPlayer* CheckActor = Cast<AFirstPersonPlayer>(OtherActor);
		if (CheckActor && DisplayWidgetClass)
		{
			LockedDoorWidget = CreateWidget<UDisplayWidget>(FPC, DisplayWidgetClass);

			//if the door is locked, set display text to Locked
			if (ActorHasTag("LockedDoor"))
			{
				LockedDoorWidget->SetText(2);
			}
			LockedDoorWidget->AddToPlayerScreen();
		}
	}
}

void ADoor::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DestroyAllWidgets();
}