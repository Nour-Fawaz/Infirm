// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Components/BoxComponent.h"
#include "Misc/App.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonController.h"
#include "Pickable.h"

// Sets default values
AGrabber::AGrabber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//component initialization
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	BoxComp->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void AGrabber::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AGrabber::OverlapBegin); //bind FUNCTION that is called to THIS object when overlapping other stuff
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AGrabber::OverlapEnd);
	
	//get FPP and FPC
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC);
	if (APC)
	{
		FPP = Cast<AFirstPersonPlayer>(APC->GetPawn());

	}

	// get socketes
	UStaticMeshComponent* GrabberMesh = GetComponentByClass<UStaticMeshComponent>();
	if (GrabberMesh)
	{
		ItemSpawnSockets = GrabberMesh->GetAllSocketNames();
	}
}

// Called every frame
void AGrabber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
* function: grabs item from player and spawns it on the grabber
*/
void AGrabber::GrabItem()
{
	if (FPP && FPC)
	{
		FPickableData EquippedData = FPP->GetEquippedItemData();

		for (FName FPPTag : EquippedData.Tags)
		{
			UE_LOG(LogTemp, Display, TEXT("is the FPPTag: %s in Items to grab: %d, does the socket map contain it: %d"),
				*FPPTag.ToString(),
				ItemsToGrab.Contains(FPPTag),
				ItemSocketMap.Contains(FPPTag));

			if (ItemsToGrab.Contains(FPPTag) && !ItemSocketMap.Contains(FPPTag) && FPC->InInventory(FPPTag))
			{
				if (!EquippedData.PickableClass)
				{
					UE_LOG(LogTemp, Warning, TEXT("No PickableClass to spawn!"));
					return;
				}

				if (CurrentSocketIndex < ItemSpawnSockets.Num())
				{
					FName SocketName = ItemSpawnSockets[CurrentSocketIndex];
					ItemSocketMap.Add(SocketName, FPPTag);

					if (!EquippedData.Mesh) // check mesh of equipped item
					{
						UE_LOG(LogTemp, Warning, TEXT("Mesh in EquippedData is null!"));
						return;
					}

					if (!StaticMesh->DoesSocketExist(SocketName)) // check socket
					{
						UE_LOG(LogTemp, Error, TEXT("Socket %s does not exist!"), *SocketName.ToString());
						return;
					}

					FVector SpawnLoc = StaticMesh->GetSocketLocation(SocketName);
					FRotator SpawnRot = StaticMesh->GetSocketRotation(SocketName);

					UStaticMeshComponent* SpawnMesh = NewObject<UStaticMeshComponent>(this, FPPTag, RF_NoFlags);
					if (SpawnMesh)
					{
						FPP->ClearEquippedItemData(); // clears data for currently equipped item

						SpawnMesh->RegisterComponent(); // set mesh to appear in world
						SpawnMesh->SetStaticMesh(EquippedData.Mesh);
						SpawnMesh->AttachToComponent(StaticMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

						// adjust actor spawn settings
						SpawnMesh->SetWorldLocation(SpawnLoc);
						SpawnMesh->SetWorldRotation(SpawnRot);
						SpawnMesh->SetVisibility(true);
						SpawnMesh->SetHiddenInGame(false);
						SpawnMesh->SetRelativeScale3D(FVector(1.0f));
						SpawnMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						SpawnMesh->SetSimulatePhysics(false);

						// remove equipped item
						FPC->RemoveFromInventory(FPPTag);
						FPP->RemoveEquippedItem();
						FPP->RemoveEquippedItemActor();

						SpawnedMeshes.Add(SpawnMesh);
						CurrentSocketIndex++;

						UE_LOG(LogTemp, Display, TEXT("Socket %s spawned with item: %s"), *SocketName.ToString(), *FPPTag.ToString());
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No more available sockets"));
				}
			}

			if (CurrentSocketIndex == ItemSpawnSockets.Num()) // check if player filled sockets available
			{
				UE_LOG(LogTemp, Warning, TEXT("GAME ENDED"));
				UGameplayStatics::OpenLevel(this, FName("EndGame"));
			}
		}
	}
}

void AGrabber::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AFirstPersonPlayer* FirstPersonPlayer = Cast<AFirstPersonPlayer>(OtherActor))
		{
			FPC->DisplayWidgetTextByInt(7); // display set down prompt to player
		}
	}
}

void AGrabber::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Display, TEXT("Overlap Ended in Grabber"));
	FPC->DestroyDisplayWidget();
}

