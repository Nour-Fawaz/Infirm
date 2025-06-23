// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawer.h"
#include "FirstPersonController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "DisplayWidget.h"


ADrawer::ADrawer()
{
	PrimaryActorTick.bCanEverTick = true;
	//set up components
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	BoxComp->SetupAttachment(StaticMesh);

}

void ADrawer::BeginPlay()
{
	Super::BeginPlay();

	//bind events
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADrawer::OverlapBegin); //bind FUNCTION that is called to THIS object when overlapping other stuff
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ADrawer::OverlapEnd);
	
	//get FPP and FPC
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC);
	if (APC)
	{
		FPP = Cast<AFirstPersonPlayer>(APC->GetPawn());
	}

	//set up movement
	OriginalLocation = GetActorLocation();
	TargetLocation = OriginalLocation;
}

void ADrawer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Opening)
	{
		TargetLocation = OriginalLocation + MoveOffset;
		FVector CurrentLocation = GetActorLocation();
		float Speed = MoveOffset.Length() / MoveTime;

		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		SetActorLocation(NewLocation);
	}

}

/*
* function: called by player to open drawer
*/
void ADrawer::OpenDrawer(ADrawer* CurrentDrawer)
{
	//player has key then unlock the drawer and open
	if (!bLocked)
	{
		Opening = true;
		if (OpenDrawerSoundEffect)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OpenDrawerSoundEffect, GetActorLocation());
		}
		DestroyAllWidgets();
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (FPP && FPP->ActorHasTag(Passkey)) 
		{
			FPC->RemoveFromInventory(Passkey); //if player uses key remove from inventory
			FPP->RemoveEquippedItem(); //remove item from player's hand
			FPP->RemoveEquippedItemActor();
		}
	}
	else if (bLocked) //if actor does not have key tell FPC to display gameplay text 
	{
		FPC->DisplayerPlayerTextWidget("KitchenDrawer");
	}
}

/*
* function: destroys active widgets related to Drawer
*/
void ADrawer::DestroyAllWidgets()
{
	if (LockedDrawerWidget)
	{
		LockedDrawerWidget->RemoveFromParent();
		LockedDrawerWidget = nullptr;
	}
}

void ADrawer::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if the player has the key unlock the drawer
	if (FPC->InInventory(Passkey))
	{
		UE_LOG(LogTemp, Display, TEXT("Actor has key"));
		DestroyAllWidgets();
		bLocked = false;
	}
	else
	{
		if (LockedDrawerWidget)
		{
			return; // already showing widget
		}

		AFirstPersonPlayer* CheckActor = Cast<AFirstPersonPlayer>(OtherActor);
		if (CheckActor && DisplayWidgetClass) 
		{
			LockedDrawerWidget = CreateWidget<UDisplayWidget>(FPC, DisplayWidgetClass); 

			//if the drawer is locked set display text to Locked
			if (bLocked)
			{
				LockedDrawerWidget->SetText(2);
			}
			LockedDrawerWidget->AddToPlayerScreen();
		}
	}
}

void ADrawer::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DestroyAllWidgets();
}

