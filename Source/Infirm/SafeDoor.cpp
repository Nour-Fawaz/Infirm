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
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	BoxComp->SetupAttachment(StaticMesh);
	


}

// Called when the game starts or when spawned
void ASafeDoor::BeginPlay()
{
	Super::BeginPlay();
	

	//BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASafeDoor::OverlapBegin); //bind FUNCTION that is called to THIS object when overlapping other stuff
	/*BoxComp->OnComponentEndOverlap.AddDynamic(this, &ASafeDoor::OverlapEnd);*/

	//get FPP and FPC
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC);
	if (APC)
	{
		FPP = Cast<AFirstPersonPlayer>(APC->GetPawn());

	}
	isOpen = false;
	

	
}

// Called every frame
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

void ASafeDoor::DisplaySafePanel(ASafeDoor* CurrentSafeDoor)
{
	if (!isOpen)
	{
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



void ASafeDoor::DestroyAllWidgets()
{
	if (SafePanelWidget)
	{
		SafePanelWidget->RemoveFromParent(); //remove widget from player viewport
		SafePanelWidget = nullptr;
		UE_LOG(LogTemp, Display, TEXT("Destroyed SafePanelWidget"))
	}
}

void ASafeDoor::OpenSafeDoor()
{
	//if (!bLocked)
	//{
	//	Opening = true;
	//	DestroyAllWidgets();
	//	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//	//remove key from player's hand IF KEY
	//	if (FPP && FPP->ActorHasTag(Passkey)) //SECOND PART CAUSES SCREWDRIVER TO REMAIN
	//	{
	//		//FPP->RemoveEquippedItem();
	//		FPC->RemoveFromInventory(Passkey); //used key -> remove from inventory
	//		this->Tags.Remove(FName("LockedDoor")); //door no longer locked
	//	}
	//}

	
	FPC->CloseSafepanelWidget(this);
	DestroyAllWidgets();
	FPC->DestroyAllWidgets();
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Opening = true;

}

//void ASafeDoor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	//if (SafePanelWidget)
//	//{
//	//	return;
//	//}
//	//AFirstPersonPlayer* CheckActor = Cast<AFirstPersonPlayer>(OtherActor);
//	//if (CheckActor && SafeDoorWidgetClass) //check actor and display widget set
//	//{
//	//	if (FPC)
//	//	{
//	//		SafePanelWidget = CreateWidget<USafeDoorWidget>(FPC, SafeDoorWidgetClass); //create widget
//	//	}
//
//	//	if (SafePanelWidget)
//	//	{
//	//		SafePanelWidget->AddToPlayerScreen(); //add to player screen
//	//	}
//
//	//}
//}

