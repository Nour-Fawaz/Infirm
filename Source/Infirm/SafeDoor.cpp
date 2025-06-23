// Fill out your copyright notice in the Description page of Project Settings.


#include "SafeDoor.h"
#include "SafeDoorWidget.h"
#include "Components/BoxComponent.h"
#include "Misc/App.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "DisplayWidget.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonController.h"
#include "Components/BoxComponent.h"

// Sets default values
ASafeDoor::ASafeDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	//initialize components
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	BoxComp->SetupAttachment(StaticMesh);
}

void ASafeDoor::BeginPlay()
{
	Super::BeginPlay();
	
	//bind events
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASafeDoor::OverlapBegin); //bind FUNCTION that is called to THIS object when overlapping other stuff
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ASafeDoor::OverlapEnd);

	//get FPP and FPC
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC);
	if (APC)
	{
		FPP = Cast<AFirstPersonPlayer>(APC->GetPawn());
	}
	isOpen = false; //all doors begin closed
}

void ASafeDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Opening)
	{
		TargetRotation = OriginalRotation + RotationOffset;
		FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
		SetActorRotation(NewRotation);
	}
}

/*
* function: displays safe panel widget for user to input code
*/
void ASafeDoor::DisplaySafePanel(ASafeDoor* CurrentSafeDoor)
{
	if (!isOpen)
	{
		FPC->DestroyDisplayWidget();
		FPC->DisplaySafePanelWidget(CurrentSafeDoor);
	}
}

void ASafeDoor::SetIsOpen(bool NewOpenStatus)
{
	isOpen = NewOpenStatus;
}

bool ASafeDoor::GetIsOpen()
{
	return isOpen;
}

FString ASafeDoor::GetSafeCode()
{
	return Code;
}

/*
* function: destroys all widgets relevant to safe door
*/
void ASafeDoor::DestroyAllWidgets()
{
	if (SafePanelWidget)
	{
		SafePanelWidget->RemoveFromParent(); //remove widget from player viewport
		SafePanelWidget = nullptr;
		UE_LOG(LogTemp, Display, TEXT("Destroyed SafePanelWidget"))
	}
}

/*
* function: called when user unlocks safe door
*/
void ASafeDoor::OpenSafeDoor()
{
	FPC->CloseSafepanelWidget(this);
	DestroyAllWidgets();
	FPC->DestroyAllWidgets();
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //disables further player interaction
	Opening = true;
}

void ASafeDoor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AFirstPersonPlayer* FirstPersonPlayer = Cast<AFirstPersonPlayer>(OtherActor))
		{
			FPC->DisplayWidgetTextByInt(8); //displays message for player to use safe
		}
	}
}

void ASafeDoor::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FPC->DestroyDisplayWidget();
}

