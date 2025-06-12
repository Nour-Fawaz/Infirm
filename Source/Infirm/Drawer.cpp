// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawer.h"
#include "PlayerTextWidget.h"
#include "FirstPersonController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "DisplayWidget.h"

// Sets default values
ADrawer::ADrawer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	BoxComp->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void ADrawer::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADrawer::OverlapBegin); //bind FUNCTION that is called to THIS object when overlapping other stuff
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ADrawer::OverlapEnd);
	
	//get FPP and FPC
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC);
	if (APC)
	{
		FPP = Cast<AFirstPersonPlayer>(APC->GetPawn());

	}

	//movement
	OriginalLocation = GetActorLocation();
	TargetLocation = OriginalLocation;
}

// Called every frame
void ADrawer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Opening)
	{
		
		TargetLocation = OriginalLocation + MoveOffset; //otherwise update the target location to be wanted target location

		FVector CurrentLocation = GetActorLocation();
		float Speed = MoveOffset.Length() / MoveTime; // diff between target and original location is move lcoation

		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		SetActorLocation(NewLocation);
	}

}

void ADrawer::OpenDrawer(ADrawer* CurrentDrawer)
{
	//if player does have garage key then unlock the drawer and open
	if (!bLocked)
	{
		Opening = true;
		if (OpenDrawerSoundEffect)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OpenDrawerSoundEffect, GetActorLocation());
		}
		if (LockedDrawerWidget)
		{
			LockedDrawerWidget->RemoveFromParent(); //remove widget from player viewport
			LockedDrawerWidget = nullptr;
		}
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//remove key from player's hand IF KEY
		if (FPP && FPP->ActorHasTag(Passkey)) //SECOND PART CAUSES SCREWDRIVER TO REMAIN
		{
			//FPP->RemoveEquippedItem();
			FPC->RemoveFromInventory(Passkey); //used key -> remove from inventory
			FPP->RemoveEquippedItem();
			//this->Tags.Remove(FName("LockedDrawer")); //door no longer locked
		}
	}
	else if (bLocked) //if actor does not have garage key tell FPC to display text saying he needs to pry this open
	{
		FPC->DisplayerPlayerTextWidget("KitchenDrawer");
	}
}

void ADrawer::DestroyAllWidgets()
{
	if (LockedDrawerWidget)
	{
		LockedDrawerWidget->RemoveFromParent(); //remove widget from player viewport
		LockedDrawerWidget = nullptr;
	}
}

void ADrawer::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("In drawer Overlap"));
	if (FPC->InInventory(Passkey))
	{
		UE_LOG(LogTemp, Display, TEXT("Actor has key"));
		if (LockedDrawerWidget)
		{
			LockedDrawerWidget->RemoveFromParent(); //remove widget from player viewport
			LockedDrawerWidget = nullptr;
		}
		bLocked = false;

	}
	else
	{
		//LockedDoorWidget = nullptr;
		if (LockedDrawerWidget != nullptr)
		{
			return; // already showing widget
		}
		AFirstPersonPlayer* CheckActor = Cast<AFirstPersonPlayer>(OtherActor);
		check(CheckActor);
		if (CheckActor && DisplayWidgetClass) //check actor and display widget set
		{
			//AFirstPersonController* FirstPersonController = CheckActor->GetController<AFirstPersonController>(); //get player controller
			check(FPC); // check if controller valid
			LockedDrawerWidget = CreateWidget<UDisplayWidget>(FPC, DisplayWidgetClass); //create widget

			//check what type of pickable
			if (bLocked)
			{
				LockedDrawerWidget->SetText(2); //set text of widget to be pickup
			}


			LockedDrawerWidget->AddToPlayerScreen(); //add to player screen

		}

	}
}

void ADrawer::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (LockedDrawerWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("Locked Door Widget being destroyed"));
		LockedDrawerWidget->RemoveFromParent(); //remove widget from player viewport
		LockedDrawerWidget = nullptr;
	}
}

