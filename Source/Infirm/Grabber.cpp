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
	//socketes
	ItemSpawnSockets = { "ItemSocketSpawn1", "ItemSocketSpawn2", "ItemSocketSpawn3", "ItemSocketSpawn4" };
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

					if (!EquippedData.Mesh)
					{
						UE_LOG(LogTemp, Warning, TEXT("Mesh in EquippedData is null!"));
						return;
					}

					if (!StaticMesh->DoesSocketExist(SocketName))
					{
						UE_LOG(LogTemp, Error, TEXT("Socket %s does not exist!"), *SocketName.ToString());
						return;
					}

					FVector SpawnLoc = StaticMesh->GetSocketLocation(SocketName);
					FRotator SpawnRot = StaticMesh->GetSocketRotation(SocketName);

					UStaticMeshComponent* SpawnMesh = NewObject<UStaticMeshComponent>(this);
					if (SpawnMesh)
					{
						FPP->ClearEquippedItemData(); // You should implement this to reset the struct
						FPC->RemoveFromInventory(FPPTag);

						SpawnMesh->RegisterComponent();
						SpawnMesh->SetStaticMesh(EquippedData.Mesh);
						SpawnMesh->AttachToComponent(StaticMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

						SpawnMesh->SetWorldLocation(SpawnLoc);
						SpawnMesh->SetWorldRotation(SpawnRot);
						SpawnMesh->SetVisibility(true);
						SpawnMesh->SetHiddenInGame(false);
						SpawnMesh->SetRelativeScale3D(FVector(1.0f));
						SpawnMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						SpawnMesh->SetSimulatePhysics(false);

						//remove equipped item
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
					UE_LOG(LogTemp, Warning, TEXT("No more available sockets to spawn items!"));
				}
			}

			if (CurrentSocketIndex == 4)
			{
				UE_LOG(LogTemp, Warning, TEXT("GAME ENDED"));
				UGameplayStatics::OpenLevel(this, FName("EndGame"));
			}
		}
	}
	//if (FPP && FPC)
	//{
	//	TArray<FName> FPPTags = FPP->GetEquippedItemActorTags();
	//	for (FName FPPTag : FPPTags)
	//	{
	//		UE_LOG(LogTemp, Display, TEXT("is the FPPTag: %s in Items to grab: %d, does the socket map contain it: %d"), *FPPTag.ToString(), ItemsToGrab.Contains(FPPTag), ItemSocketMap.Contains(FPPTag));
	//		if (ItemsToGrab.Contains(FPPTag) && !ItemSocketMap.Contains(FPPTag) && FPC->InInventory(FPPTag))
	//		{
	//			APickable* PickableToSpawn = FPP->GetEquippedItemActor();

	//			//if PickableToSpawn is valid and we have a socket available
	//			if (!PickableToSpawn)
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("No pickable item to spawn!"));
	//				return;
	//			}
	//			UE_LOG(LogTemp, Display, TEXT("Current Socket Index: %d"), CurrentSocketIndex);

	//			if (PickableToSpawn && CurrentSocketIndex < ItemSpawnSockets.Num()) // if PickableToSpawn is valid and there are enough sockets
	//			{
	//				ItemSocketMap.Add(ItemSpawnSockets[CurrentSocketIndex], FPPTag); //map tag to socket
	//				
	//				//get mesh of PickableToSpawn
	//				UStaticMeshComponent* SourceMeshComp = Cast<UStaticMeshComponent>(PickableToSpawn->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	//				if (!SourceMeshComp) return;
	//			    MeshToSpawn = SourceMeshComp->GetStaticMesh();
	//				if (!MeshToSpawn) return;

	//				FName SocketName = ItemSpawnSockets[CurrentSocketIndex]; //get socket name
	//				if (!StaticMesh->DoesSocketExist(SocketName)) //check if socket exists
	//				{
	//					UE_LOG(LogTemp, Error, TEXT("Socket %s does not exist!"), *SocketName.ToString());
	//					return;
	//				}

	//				//spawn
	//				FVector SpawnLoc = StaticMesh->GetSocketLocation(SocketName);
	//				FRotator SpawnRot = StaticMesh->GetSocketRotation(SocketName);
	//				UStaticMeshComponent* SpawnMesh = NewObject<UStaticMeshComponent>(this);
	//				if (SpawnMesh)
	//					{
	//					    FPP->RemoveEquippedItem();
	//					    FPC->RemoveFromInventory(FPPTag);
	//						SpawnMesh->RegisterComponent();
	//						SpawnMesh->SetStaticMesh(MeshToSpawn);
	//					    SpawnMesh->AttachToComponent(StaticMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	//						UE_LOG(LogTemp, Display, TEXT("Socket %s spawned with item: %s"), *SocketName.ToString(), *FPPTag.ToString());
	//						//setup spawn mesh properties
	//						SpawnMesh->SetWorldLocation(SpawnLoc);
	//						SpawnMesh->SetWorldRotation(SpawnRot);
	//						SpawnMesh->SetVisibility(true);
	//						SpawnMesh->SetHiddenInGame(false);
	//						SpawnMesh->SetRelativeScale3D(FVector(1.0f));
	//						SpawnMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//						SpawnMesh->SetSimulatePhysics(false);

	//						// Store reference to prevent garbage collection
	//						SpawnedMeshes.Add(SpawnMesh);

	//						CurrentSocketIndex++;
	//						FPP->RemoveEquippedItemActor();
	//				}
	//			}
	//			else
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("No more available sockets to spawn items!"));
	//			}
	//		}
	//		if (CurrentSocketIndex == 4) //player collected all items, game ended
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("GAME ENDED"));
	//			UGameplayStatics::OpenLevel(this, FName("EndGame"));
	//		}
	//	}
	//}
}

void AGrabber::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AFirstPersonPlayer* FirstPersonPlayer = Cast<AFirstPersonPlayer>(OtherActor))
		{
			FPC->DisplayWidgetTextByInt(7);
		}
	}
}

void AGrabber::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Display, TEXT("Overlap Ended in Grabber"));
	FPC->DestroyDisplayWidget();
}

