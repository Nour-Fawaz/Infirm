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

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	BoxComp->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = this->GetActorRotation();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OverlapBegin); //bind FUNCTION that is called to THIS object when overlapping other stuff
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ADoor::OverlapEnd);

	//get FPP and FPC
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC);
	if (APC)
	{
		FPP = Cast<AFirstPersonPlayer>(APC->GetPawn());

	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Opening)
	{
		//UKismetSystemLibrary::PrintString(this, TEXT("in tick moving"), true, true, FColor::Cyan, 5.0f);
		TargetRotation = OriginalRotation + RotationOffset;
		FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
		SetActorRotation(NewRotation);

	}

}

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

		//remove key from player's hand IF KEY
		if (FPP && FPP->ActorHasTag(Passkey)) //SECOND PART CAUSES SCREWDRIVER TO REMAIN
		{
			//FPP->RemoveEquippedItem();
			FPC->RemoveFromInventory(Passkey); //used key -> remove from inventory
			this->Tags.Remove(FName("LockedDoor")); //door no longer locked
		}
	}
}

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
	UKismetSystemLibrary::PrintString(this, TEXT("iN OVERLAP."), true, true, FColor::Cyan, 5.0f);
	if (FPC->InInventory(Passkey))
	{
		UE_LOG(LogTemp, Display, TEXT("Actor has key"));
		DestroyAllWidgets();
		bLocked = false;
		
	}
	else
	{
		//LockedDoorWidget = nullptr;
		if (LockedDoorWidget != nullptr)
		{
			return; // already showing widget
		}
		AFirstPersonPlayer* CheckActor = Cast<AFirstPersonPlayer>(OtherActor);
		check(CheckActor);
		if (CheckActor && DisplayWidgetClass) //check actor and display widget set
		{
			//AFirstPersonController* FirstPersonController = CheckActor->GetController<AFirstPersonController>(); //get player controller
			check(FPC); // check if controller valid
			LockedDoorWidget = CreateWidget<UDisplayWidget>(FPC, DisplayWidgetClass); //create widget

			//check what type of pickable
			if (ActorHasTag("LockedDoor"))
			{
				LockedDoorWidget->SetText(2); //set text of widget to be pickup
			}


			check(LockedDoorWidget); //check widget is valiud
			LockedDoorWidget->AddToPlayerScreen(); //add to player screen

		}

	}
}

void ADoor::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (LockedDoorWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("Locked Door Widget being destroyed"));
		LockedDoorWidget->RemoveFromParent(); //remove widget from player viewport
		LockedDoorWidget = nullptr;
	}
}





